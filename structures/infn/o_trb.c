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
#include <sys/time.h>
const char v_trb_kind[] = "trb";
void p_trb_hooking(struct o_trb *object) {
  object->head.s_delegate.m_delete = p_trb_delete;
  object->head.s_delegate.m_compare = p_trb_compare;
  object->head.s_delegate.m_hash = p_trb_hash;
  object->head.s_delegate.m_string = p_trb_string;
  /* keep default clone function */
  object->m_led = p_trb_led;
  object->m_setup = p_trb_setup;
  object->m_stop = p_trb_stop;
  object->m_close_stream = p_trb_close_stream;
  object->m_stream = p_trb_stream;
  object->m_event = p_trb_event;
}

int p_trb_read(struct o_trb *object, unsigned char *data, size_t size, time_t timeout) {
  struct timeval current_timestamp;
  long long current_bunk, elapsed_bunk;
  int result = d_false;
  size_t bytes_bunk;
  if (object->handler)
    result = usb_bulk_read(object->handler, object->read_address, (char *)data, size, timeout);
  else if (((object->stream_in) && (object->stream_in->s_flags.opened)) && (object->trigger_on)) {
    gettimeofday(&current_timestamp, NULL);
    current_bunk = (1000000u*(long long)current_timestamp.tv_sec)+current_timestamp.tv_usec;
    if ((elapsed_bunk = (current_bunk-object->last_bunk)) >= object->time_bunk) {
      if ((bytes_bunk = (elapsed_bunk/object->time_bunk)) > size)
        bytes_bunk = size;
      object->last_bunk = current_bunk;
      result = read(object->stream_in->descriptor, data, bytes_bunk);
    }
  }
  return result;
}

int p_trb_write(struct o_trb *object, unsigned char *data, size_t size, time_t timeout) {
  int result = d_false;
  if (object->handler)
    result = usb_bulk_write(object->handler, object->write_address, (char *)data, size, timeout);
  else if ((object->stream_in) && (object->stream_in->s_flags.opened))
    result = d_true;
  return result;
}

int p_trb_check(struct usb_device *device, struct usb_dev_handle *handler, struct o_stream *stream_in) {
  int result = d_false;
  char manufacturer[d_string_buffer_size] = {0}, product[d_string_buffer_size] = {0};
  if (handler) {
    usb_get_string_simple(handler, device->descriptor.iManufacturer, manufacturer, d_string_buffer_size);
    usb_get_string_simple(handler, device->descriptor.iProduct, product, d_string_buffer_size);
    if ((strncmp(d_trb_manufacturer_label, manufacturer, strlen(d_trb_manufacturer_label)) == 0) &&
        (strncmp(d_trb_product_label, product, strlen(d_trb_product_label)) == 0))
      result = d_true;
  } else if ((stream_in) && (stream_in->s_flags.opened))
    result = d_true;
  return result;
}

struct o_trb *f_trb_new(struct o_trb *supplied, struct usb_device *device, struct usb_dev_handle *handler) {
  struct o_trb *result;
  struct usb_interface *interface = device->config->interface;
  struct usb_interface_descriptor *descriptor = interface->altsetting;
  int index;
  unsigned char address;
  if ((result = (struct o_trb *) f_object_new(v_trb_kind, sizeof(struct o_trb), (struct o_object *)supplied))) {
    p_trb_hooking(result);
    result->led_status = 0x00;
    result->device = device;
    result->handler = handler;
    result->stream_lock = f_object_new_pure(NULL);
    if (p_trb_check(result->device, result->handler, NULL)) {
      if (usb_set_configuration(result->handler, 1) >= 0) {
        if (usb_claim_interface(result->handler, 0) >= 0) {
          /* dinamically compute endpoints */
          for (index = 0; index < descriptor->bNumEndpoints; ++index) {
            address = descriptor->endpoint[index].bEndpointAddress;
            if (address&0x80)
              result->read_address = address;
            else
              result->write_address = address;
          }
        } else
          d_throw(v_exception_unsupported, "unable to claim the USB interface");
      } else
        d_throw(v_exception_unsupported, "unable to set an active configuration");
    } else
      d_throw(v_exception_incompatible, "the device isn't a miniTBR interface");
  }
  return result;
}

struct o_trb *f_trb_new_file(struct o_trb *supplied, struct o_stream *stream, float frequency) {
  struct o_trb *result;
  if ((result = (struct o_trb *) f_object_new(v_trb_kind, sizeof(struct o_trb), (struct o_object *)supplied))) {
    p_trb_hooking(result);
    result->stream_in = stream;
    result->frequency = frequency;
    result->time_bunk = (1000000.0f/frequency);
    result->stream_lock = f_object_new_pure(NULL);
    if (!result->stream_in->s_flags.opened)
      d_throw(v_exception_unsupported, "unable to read from the source binary");
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
    if (local_object->stream_in)
      d_release(local_object->stream_in);
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
    if (local_object->handler)
      written = snprintf(data, size, "<trb interface>");
    else if (local_object->stream_in)
      written = snprintf(data, size, "<trb interface (playback)>");
    else
      written = snprintf(data, size, "<trb interface (empty)>");
    written = ((written>size)?size:written);
    data += written;
  } else
    d_throw(v_exception_kind, "object is not an instance of o_trb");
  return data;
}

int p_trb_led(struct o_trb *object, time_t timeout) {
  int result = d_false;
  unsigned char manage_led[] = {0x00, 0xc0, 0x00, 0x00, object->led_status, 0x00};
  if (object->handler)
    if ((result = p_trb_write(object, manage_led, sizeof(manage_led), timeout)))
      object->led_status = (object->led_status == 0x11)?0x00:0x11;
  return result;
}

int p_trb_setup(struct o_trb *object, unsigned char trigger, float hold_delay, enum e_trb_mode mode, unsigned short dac, unsigned char channel, time_t timeout) {
  int result = d_false, discarded = 0;
  unsigned char setup_command[] = {0x00, 0xb0, 0x00, 0x00, 0x00, trigger}, startup_command[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
                enable_trigger[] = {0x00, 0xd0, 0x00, 0x00, 0x11, 0x00}, disable_trigger[] = {0x00, 0xd0, 0x00, 0x00, 0x00, 0x00},
                buffer[d_trb_packet_size], dac_h, dac_l, extra_package[] = {0xde, 0xad, 0xbe, 0xef, 0x00, 0x00, 0x00};
  if (p_trb_check(object->device, object->handler, object->stream_in)) {
    setup_command[4] = (unsigned int)((float)hold_delay/0.05);
    dac_h = (unsigned char)(dac>>8)&0xff;
    dac_l = (unsigned char)(dac)&0xff;
    switch (mode) {
      case e_trb_mode_normal:
        startup_command[1] = 0xa0;
        object->event_size = d_trb_event_size_normal;
        break;
      case e_trb_mode_calibration:
        startup_command[1] = 0xa1;
        object->event_size = d_trb_event_size_normal;
        startup_command[4] = dac_l;
        startup_command[5] = dac_h;
        break;
      case e_trb_mode_calibration_software:
      case e_trb_mode_calibration_debug_digital:
        startup_command[1] = 0xa3;
        object->event_size = d_trb_event_size_debug;
#ifdef d_version_0x1313
        startup_command[2] = (channel&0xFF);
        startup_command[3] = ((channel>>8)&0x1);
#else
        startup_command[2] = channel;
#endif
        startup_command[4] = dac_l;
        startup_command[5] = dac_h;
        /* to correctly understand the package configuration */
        extra_package[4] = channel;
        extra_package[5] = dac_l;
        extra_package[6] = dac_h;
        break;
    }
    object->time_bunk = (1000000.0/(object->event_size*(object->frequency+1)));
    object->kind = startup_command[1];
    if ((result = p_trb_write(object, disable_trigger, sizeof(disable_trigger), timeout)) > 0) {
      object->trigger_on = d_false;
      if (object->handler)
        while ((result = p_trb_read(object, buffer, d_trb_packet_size, d_trb_buffer_timeout)) > 0)
          discarded++;
      if ((result = p_trb_write(object, setup_command, sizeof(setup_command), timeout)) > 0)
        if ((result = p_trb_write(object, startup_command, sizeof(startup_command), timeout)) > 0) {
          result = p_trb_write(object, enable_trigger, sizeof(enable_trigger), timeout);
          object->trigger_on = d_true;
          if ((mode == e_trb_mode_calibration_software) || (mode == e_trb_mode_calibration_debug_digital)) {
            d_object_lock(object->stream_lock);
            if (object->stream_out)
              object->stream_out->m_write(object->stream_out, sizeof(extra_package), extra_package);
            d_object_unlock(object->stream_lock);
          }
        }
    }
  }
  object->last_error = result;
  return result;
}

int p_trb_stop(struct o_trb *object, time_t timeout) {
  int result = d_false;
  unsigned char disable_trigger[] = {0x00, 0xd0, 0x00, 0x00, 0x00, 0x00}, setup_command[] = {0x00, 0xb0, 0x00, 0x00, 0x00, 0x22},
                startup_command[] = {0x00, 0xa0, 0x00, 0x00, 0x00, 0x00};
  if (object->handler) {
    if ((result = p_trb_write(object, disable_trigger, sizeof(disable_trigger), timeout)) > 0)
      if ((result = p_trb_write(object, setup_command, sizeof(setup_command), timeout)) > 0)
        result = p_trb_write(object, startup_command, sizeof(startup_command), timeout);
  }
  object->trigger_on = d_false;
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
  if (p_trb_check(object->device, object->handler, object->stream_in)) {
    if (!result)
      result = f_trb_event_new(NULL);
    result->filled = d_false;
    while ((object->buffer_fill >= object->event_size) && (!result->filled)) {
      if (result->m_load(result, object->buffer, object->kind, object->buffer_fill)) {
        object->buffer_fill -= object->event_size;
        memmove(object->buffer, (object->buffer+object->event_size), object->buffer_fill);
      } else
        object->buffer_fill = p_trb_event_align(object->buffer, object->buffer_fill);
    }
    pointer = (unsigned char *)object->buffer+object->buffer_fill;
    if ((d_trb_buffer_size-object->buffer_fill) >= d_trb_packet_size)
      if ((readed = p_trb_read(object, pointer, d_trb_packet_size, timeout)) > 0) {
        object->buffer_fill += readed;
        d_object_lock(object->stream_lock);
        if (object->stream_out)
          object->stream_out->m_write(object->stream_out, readed, (void *)pointer);
        d_object_unlock(object->stream_lock);
      }
  }
  object->last_error = readed;
  return result;
}

