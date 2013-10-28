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
#include "o_trbs.h"
#include <unistd.h>
#include <stdlib.h>
const char v_trbs_kind[] = "trbs";
int v_trbs_usb_init = d_false;
void p_trbs_hooking(struct o_trbs *object) {
	object->head.s_delegate.m_delete = p_trbs_delete;
	/* keep default compare function */
	/* keep default hash function */
	object->head.s_delegate.m_string = p_trbs_string;
	/* keep default clone function */
	object->m_async_search = p_trbs_async_search;
}

int p_trbs_thread_continue(struct o_trbs *object) {
	int result = d_true;
	if (d_object_trylock(object->semaphore)) {
		d_object_unlock(object->semaphore);
		result = d_false;
	}
	return result;
}

int p_trbs_thread_already(struct o_trbs *object, struct usb_device *device) {
	int result = d_false, index;
	for (index = 0; index < d_trbs_slots_size; index++)
		if (object->devices[index].device)
			/* no idea, we have to check */
			if (object->devices[index].device == device) {
				object->devices[index].referenced = d_true;
				result = d_true;
				break;
			}
	return result;
}

void *p_trbs_thread(void *parameters) {
	struct usb_bus *bus;
	struct usb_device *device;
	struct usb_dev_handle *handler;
	struct s_trbs_parameters *local_parameters = (struct s_trbs_parameters *)parameters;
	int index;
	while ((p_trbs_thread_continue(local_parameters->object)) && (usleep(local_parameters->sleep)==0))
		if ((usb_find_busses()) | (usb_find_devices())) {
			for (bus = usb_get_busses(); bus; bus = bus->next)
				for (device = bus->devices; device && (!p_trbs_thread_already(local_parameters->object, device)); device = device->next) {
					for (index = 0; index < d_trbs_slots_size; index++)
						if (!local_parameters->object->devices[index].device) {
							local_parameters->object->devices[index].device = device;
							local_parameters->object->devices[index].referenced = d_true;
						}
					if ((handler = usb_open(device))) {
						if ((p_trb_check(device, handler)))
							local_parameters->handle(f_trb_new(NULL, device, handler));
						else
							usb_close(handler);
					}
				}
			for (index = 0; index < d_trbs_slots_size; index++) {
				if (local_parameters->object->devices[index].device)
					if (!local_parameters->object->devices[index].referenced)
						local_parameters->object->devices[index].device = NULL;
				local_parameters->object->devices[index].referenced = d_false;
			}
		}
	pthread_exit(NULL);
}

struct o_trbs *f_trbs_new(struct o_trbs *supplied) {
	struct o_trbs *result;
	if ((result = (struct o_trbs *) f_object_new(v_trbs_kind, sizeof(struct o_trbs), (struct o_object *)supplied))) {
		p_trbs_hooking(result);
		result->thread_id = pthread_self(); /* is empty */
		if ((result->semaphore = f_object_new_pure(NULL))) {
			if (!v_trbs_usb_init) {
				usb_init();
				v_trbs_usb_init = d_true;
			}
		} else
			d_die(d_error_malloc);
	}
	return result;
}

void p_trbs_delete(struct o_object *object) {
	struct o_trbs *local_object;
	void *result;
	if ((local_object = d_object_kind(object, trbs))) {
		if (!pthread_equal(local_object->thread_id, pthread_self())) {
			d_object_unlock(local_object->semaphore);
			pthread_join(local_object->thread_id, &result);
		}
		d_release(local_object->semaphore);
	} else
		d_throw(v_exception_kind, "object is not an instance of o_trbs");
}

char *p_trbs_string(struct o_object *object, char *data, size_t size) {
	struct o_trbs *local_object;
	size_t written;
	if ((local_object = d_object_kind(object, trbs))) {
		written = snprintf(data, size, "<trbs interface>");
		written = ((written>size)?size:written);
		data += written;
	} else
		d_throw(v_exception_kind, "object is not an instance of o_trbs");
	return data;
}

int p_trbs_async_search(struct o_trbs *object, t_trbs_handle *handle, time_t sleep) {
	int result = d_false;
	struct s_trbs_parameters *parameters;
	if (pthread_equal(object->thread_id, pthread_self())) {
		if ((parameters = (struct s_trbs_parameters *) d_malloc(sizeof(struct s_trbs_parameters)))) {
			parameters->object = object;
			parameters->handle = handle;
			parameters->sleep = sleep;
			d_object_lock(object->semaphore);
			if ((pthread_create(&(object->thread_id), NULL, p_trbs_thread, parameters)) == 0)
				result = d_true;
			else
				object->thread_id = pthread_self();
		} else
			d_die(d_error_malloc);
	}
	return result;
}
