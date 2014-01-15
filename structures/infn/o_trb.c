/*
 * serenity
 * Copyright (C) 2013 Andrea Nardinocchi (andrea@nardinan.it)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "o_trb.h"
#include <unistd.h>
#include <stdlib.h>
const char v_trb_kind[] = "trb";
void p_trb_hooking(struct o_trb *object) {
	object->head.s_delegate.m_delete = p_trb_delete;
	object->head.s_delegate.m_compare = p_trb_compare;
	object->head.s_delegate.m_hash = p_trb_hash;
	object->head.s_delegate.m_string = p_trb_string;
	/* keep default clone function */
	object->m_setup = p_trb_setup;
	object->m_stop = p_trb_stop;
	object->m_close_stream = p_trb_close_stream;
	object->m_stream = p_trb_stream;
	object->m_event = p_trb_event;
}

int p_trb_read(struct o_trb *object, unsigned char *data, size_t size, time_t timeout) {
	int result = d_false;
	if (object->handler)
		result = usb_bulk_read(object->handler, object->read_address, (char *)data, size, timeout);
	return result;
}

int p_trb_write(struct o_trb *object, unsigned char *data, size_t size, time_t timeout) {
	int result = d_false, index;
	if (object->handler) {
		result = usb_bulk_write(object->handler, object->write_address, (char *)data, size, timeout);
		printf("WR[");
		for (index = 0; index < size; index++)
			printf("0x%x ", data[index]);
		printf("] (%d)\n", result);
	}
	return result;
}

int p_trb_check(struct usb_device *device, struct usb_dev_handle *handler) {
	int result = d_false;
	char manufacturer[d_string_buffer_size] = {0}, product[d_string_buffer_size] = {0};
	usb_get_string_simple(handler, device->descriptor.iManufacturer, manufacturer, d_string_buffer_size);
	usb_get_string_simple(handler, device->descriptor.iProduct, product, d_string_buffer_size);
	if ((strncmp(d_trb_manufacturer_label, manufacturer, strlen(d_trb_manufacturer_label)) == 0) &&
			(strncmp(d_trb_product_label, product, strlen(d_trb_product_label)) == 0))
		result = d_true;
	return result;
}

struct o_trb *f_trb_new(struct o_trb *supplied, struct usb_device *device, struct usb_dev_handle *handler) {
	struct o_trb *result;
	if ((result = (struct o_trb *) f_object_new(v_trb_kind, sizeof(struct o_trb), (struct o_object *)supplied))) {
		p_trb_hooking(result);
		result->device = device;
		result->handler = handler;
		result->stream_lock = f_object_new_pure(NULL);
		if (p_trb_check(result->device, result->handler)) {
			if (usb_set_configuration(result->handler, 1) >= 0) {
				if (usb_claim_interface(result->handler, 0) >= 0) {
					result->write_address = d_trb_write_endpoint;
					result->read_address = d_trb_read_endpoint;
				} else
					d_throw(v_exception_unsupported, "unable to claim the USB interface");
			} else
				d_throw(v_exception_unsupported, "unable to set an active configuration");
		} else
			d_throw(v_exception_incompatible, "the device isn't a miniTBR interface");
	}
	return result;
}

int p_trb_compare(struct o_object *object, struct o_object *other) {
	struct o_trb *local_object, *local_other;
	int result = p_object_compare(object, other);
	if ((local_object = d_object_kind(object, trb)) && (local_other = d_object_kind(other, trb)))
		result = local_object->device-local_other->device;
	return result;
}

void p_trb_delete(struct o_object *object) {
	struct o_trb *local_object;
	if ((local_object = d_object_kind(object, trb))) {
		if (local_object->stream_out)
			d_release(local_object->stream_out);
		d_release(local_object->stream_lock);
	} else
		d_throw(v_exception_kind, "object is not an instance of o_trb");
}

t_hash_value p_trb_hash(struct o_object *object) {
	struct o_trb *local_object;
	t_hash_value result = 0;
	if ((local_object = d_object_kind(object, trb))) {
		if (!object->s_flags.hashed) {
			object->hash = 5381;
			/* djb2 hash function */
			object->hash = ((object->hash<<5)+object->hash)+(t_hash_value)local_object->device;
			object->s_flags.hashed = d_true;
		}
		result = object->hash;
	}  else
		d_throw(v_exception_kind, "object is not an instance of o_trb");
	return result;
}

char *p_trb_string(struct o_object *object, char *data, size_t size) {
	struct o_trb *local_object;
	size_t written;
	if ((local_object = d_object_kind(object, trb))) {
		written = snprintf(data, size, "<trb interface>");
		written = ((written>size)?size:written);
		data += written;
	} else
		d_throw(v_exception_kind, "object is not an instance of o_trb");
	return data;
}

int p_trb_setup(struct o_trb *object, unsigned char trigger, float hold_delay, enum e_trb_mode mode, unsigned char dac, unsigned char channel, time_t timeout) {
	int result = d_false;
	unsigned char setup_command[] = {0x00, 0xb0, 0x00, 0x00, 0x00, trigger}, startup_command[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		      enable_trigger[] = {0x00, 0xd0, 0x00, 0x00, 0x11, 0x00};
	if (object->handler) {
		if ((hold_delay >= 3.0) && (hold_delay <= 10.0)) {
			setup_command[4] = ((float)hold_delay/0.05);
			switch (mode) {
				case e_trb_mode_normal:
					startup_command[1] = 0xa0;
					object->event_size = d_trb_event_size_normal;
					break;
				case e_trb_mode_calibration:
					startup_command[1] = 0xa1;
					object->event_size = d_trb_event_size_normal;
					startup_command[4] = (((float)dac/1024.0f)*4096.0f)/2.0f; /* just a copypaste from Zhang Fei's implementation */
					break;
				case e_trb_mode_calibration_debug_digital:
					startup_command[1] = 0xa3;
					object->event_size = d_trb_event_size_debug;
					startup_command[4] = (((float)dac/1024.0f)*4096.0f)/2.0f; /* just a copypaste from Zhang Fei's implementation */
					startup_command[5] = channel;
					break;
			}
			object->kind = startup_command[1];
			if ((result = p_trb_write(object, setup_command, sizeof(setup_command), timeout)) > 0)
				if ((result = p_trb_write(object, startup_command, sizeof(startup_command), timeout)) > 0)
					result = p_trb_write(object, enable_trigger, sizeof(enable_trigger), timeout);
		}
	}
	object->last_error = result;
	return result;
}

int p_trb_stop(struct o_trb *object, time_t timeout) {
	int result = d_false;
	unsigned char disable_trigger[] = {0x00, 0xd0, 0x00, 0x00, 0x00, 0x00};
	if (object->handler)
		result = p_trb_write(object, disable_trigger, sizeof(disable_trigger), timeout);
	object->last_error = result;
	return result;
}

void p_trb_close_stream(struct o_trb *object) {
	d_object_lock(object->stream_lock);
	if (object->stream_out)
		d_release(object->stream_out);
	object->stream_out = NULL;
	d_object_unlock(object->stream_lock);
}

void p_trb_stream(struct o_trb *object, struct o_stream *supplied, struct o_string *name, const char *action, int permission) {
	p_trb_close_stream(object);
	d_object_lock(object->stream_lock);
	object->stream_out = f_stream_new_file(supplied, name, action, permission);
	d_object_unlock(object->stream_lock);
}

struct o_trb_event *p_trb_event(struct o_trb *object, struct o_trb_event *provided, time_t timeout) {
	struct o_trb_event *result = provided;
	unsigned char *pointer;
	ssize_t readed = d_false;
	if (object->handler) {
		if (!result)
			result = f_trb_event_new(NULL);
		result->filled = d_false;
		while ((object->buffer_fill >= object->event_size) && (!result->filled))
			if (result->m_load(result, object->buffer, object->kind, object->buffer_fill)) {
				object->buffer_fill -= object->event_size;
				memmove(object->buffer, (object->buffer+object->event_size), object->buffer_fill);
			} else
				object->buffer_fill = p_trb_event_align(object->buffer, object->buffer_fill);
			pointer = (unsigned char *)object->buffer+object->buffer_fill;
			if ((d_trb_buffer_size-object->buffer_fill) >= d_trb_packet_size) {
				if ((readed = p_trb_read(object, pointer, d_trb_packet_size, timeout))) {
					object->buffer_fill += readed;
					d_object_lock(object->stream_lock);
					if (object->stream_out)
						object->stream_out->m_write(object->stream_out, readed, (void *)pointer);
					d_object_unlock(object->stream_lock);
				}
			}
	}
	object->last_error = readed;
	return result;
}

