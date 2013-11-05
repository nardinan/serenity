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
#include "o_trb_event.h"
#include <unistd.h>
#include <stdlib.h>
const char v_trb_event_kind[] = "trb_event";
void p_trb_event_hooking(struct o_trb_event *object) {
	/* keep default delete function */
	object->head.s_delegate.m_compare = p_trb_event_compare;
	object->head.s_delegate.m_hash = p_trb_event_hash;
	object->head.s_delegate.m_string = p_trb_event_string;
	/* keep default clone function */
	object->m_load = p_trb_event_load;
}

unsigned int p_trb_event_align(unsigned char *buffer, size_t size) {
	size_t index, result = 0;
	for (index = 0; index < size; index++)
		if ((buffer[index] == 0x90) && ((index == (size-1)) || (buffer[index+1] == 0xeb))) {
			result = size-index;
			memmove(buffer, &(buffer[index]), result);
			break;
		}
	return result;
}

struct o_trb_event *f_trb_event_new(struct o_trb_event *supplied) {
	struct o_trb_event *result;
	if ((result = (struct o_trb_event *) f_object_new(v_trb_event_kind, sizeof(struct o_trb_event), (struct o_object *)supplied)))
		p_trb_event_hooking(result);
	return result;
}

int p_trb_event_compare(struct o_object *object, struct o_object *other) {
	struct o_trb_event *local_object, *local_other;
	int result = p_object_compare(object, other), index = 0;
	if ((local_object = d_object_kind(object, trb_event)) && (local_other = d_object_kind(other, trb_event)))
		if (((result = local_object->code-local_other->code) == 0) && ((result = local_object->filled-local_other->filled) == 0) &&
				((result = local_object->kind-local_other->kind) == 0) &&
				((result = local_object->temperature[0]-local_other->temperature[1]) == 0) &&
				((result = local_object->temperature[1]-local_other->temperature[1]) == 0))
			while ((index < d_trb_event_channels) && (result == 0)) {
				result = local_object->values[index]-local_other->values[index];
				index++;
			}
	return result;
}

t_hash_value p_trb_event_hash(struct o_object *object) {
	struct o_trb_event *local_object;
	t_hash_value result = 0;
	if ((local_object = d_object_kind(object, trb_event))) {
		if (!object->s_flags.hashed) {
			object->hash = 5381;
			/* djb2 hash function */
			object->hash = ((object->hash<<5)+object->hash)+(t_hash_value)local_object->code;
			object->hash = ((object->hash<<5)+object->hash)+(t_hash_value)local_object->kind;
			object->s_flags.hashed = d_true;
		}
		result = object->hash;
	}  else
		d_throw(v_exception_kind, "object is not an instance of o_trb_event");
	return result;
}

char *p_trb_event_string(struct o_object *object, char *data, size_t size) {
	struct o_trb_event *local_object;
	size_t written;
	if ((local_object = d_object_kind(object, trb_event))) {
		written = snprintf(data, size, "<trb event 0x%x kind 0x%x>", local_object->code, local_object->kind);
		written = ((written>size)?size:written);
		data += written;
	} else
		d_throw(v_exception_kind, "object is not an instance of o_trb_event");
	return data;
}

unsigned char *p_trb_event_load(struct o_trb_event *object, unsigned char *raw_data, size_t size) {
	unsigned char *result = NULL;
	int index, channel;
	object->filled = d_false;
	if (size >= d_trb_event_size_minimum)
		if (d_trb_event_header(raw_data)) {
			object->kind = raw_data[2];
			if (((object->kind == 0xa3) && (size >= d_trb_event_size_debug)) || (size >= d_trb_event_size_normal)) {
				object->code = raw_data[3];
				for (index = 0, result = (raw_data+4); index < d_trb_event_channels; index++, result += 2) {
					channel = ((index%2)*d_trb_event_channels_half)+(index/2);
					object->values[channel] = ((unsigned short int)result[0])|((unsigned short int)result[1])<<8;
				}
				object->temperature[0] = ((unsigned short int)result[0])|((unsigned short int)result[1])<<8;
				result += 2;
				object->temperature[1] = ((unsigned short int)result[0])|((unsigned short int)result[1])<<8;
				result += 2;
				object->filled = d_true;
			}
		}
	return result;
}

