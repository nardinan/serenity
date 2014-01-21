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
#include "o_trb.h"
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
	size_t index, result;
	for (index = 1; index < size; index++)
		if ((buffer[index] == 0x90) && ((index == (size-1)) || (buffer[index+1] == 0xeb)))
			break;
	if ((result = size-index))
		memmove(buffer, &(buffer[index]), result);
	return result;
}

float *p_trb_event_pedestal(struct o_trb_event *events, size_t size, float *supplied) {
	float *result = supplied;
	int channel, event;
	if (!result)
		if (!(result = (float *) d_calloc(sizeof(float)*d_trb_event_channels, 1)))
			d_die(d_error_malloc);
	for (channel = 0; channel < d_trb_event_channels; channel++) {
		result[channel] = 0;
		for (event = 0; event < size; event++)
			result[channel] += events[event].values[channel];
		result[channel] = (result[channel]/(float)size);
	}
	return result;
}

float *p_trb_event_sigma_raw(struct o_trb_event *events, size_t size, float *supplied) {
	float *result = supplied, total, total_square, fraction = (1.0/(float)size);
	int channel, event;
	if (!result)
		if (!(result = (float *) d_calloc(sizeof(float)*d_trb_event_channels, 1)))
			d_die(d_error_malloc);
	for (channel = 0; channel < d_trb_event_channels; channel++) {
		for (event = 0, total = 0, total_square = 0; event < size; event++) {
			total += events[event].values[channel];
			total_square += events[event].values[channel]*events[event].values[channel];
		}
		total *= fraction;
		total_square *= fraction;
		result[channel] = sqrt(fabs(total_square-(total*total)));
	}
	return result;
}

float *p_trb_event_sigma(struct o_trb_event *events, size_t size, float sigma_multiplicator, float *sigma_raw, float *pedestal, int *flags, float *supplied) {
	float *result = supplied, common_noise[d_trb_event_vas], common_noise_va, common_noise_pure[d_trb_event_channels] = {0},
	      common_noise_pure_square[d_trb_event_channels] = {0}, value, fraction;
	int va, channel, local_channel, event, entries, local_entries[d_trb_event_channels] = {0};
	if (!result)
		if (!(result = (float *) d_calloc(sizeof(float)*d_trb_event_channels, 1)))
			d_die(d_error_malloc);
	for (event = 0; event < size; event++) {
		for (va = 0, channel = 0; va < d_trb_event_vas; va++, channel += d_trb_event_channels_on_va) {
			for (local_channel = channel, common_noise[va] = 0, common_noise_va = 0, entries = 0;
					local_channel < (channel+d_trb_event_channels_on_va); local_channel++)
				if ((!flags) || ((flags[local_channel]&e_trb_event_channel_damaged) != e_trb_event_channel_damaged)) {
					value = ((float)(events[event].values[local_channel]))-pedestal[local_channel];
					if (fabs(value) < sigma_multiplicator*sigma_raw[local_channel]) {
						common_noise_va += value;
						entries++;
					}
				}
			if (entries)
				common_noise[va] = common_noise_va/(float)entries;
		}
		for (channel = 0; channel < d_trb_event_channels; channel++) {
			value = ((float)(events[event].values[channel]))-pedestal[channel]-common_noise[(int)(channel/d_trb_event_channels_on_va)];
			if (fabs(value) < (sigma_multiplicator*sigma_raw[channel])) {
				common_noise_pure[channel] += value;
				common_noise_pure_square[channel] += value*value;
				local_entries[channel]++;
			}
		}
	}
	for (channel = 0; channel < d_trb_event_channels; channel++)
		if (local_entries[channel]) {
			fraction = (1.0/(float)local_entries[channel]);
			common_noise_pure[channel] *= fraction;
			common_noise_pure_square[channel] *= fraction;
			result[channel] = sqrt(fabs(common_noise_pure_square[channel]-(common_noise_pure[channel]*common_noise_pure[channel])));
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
				((result = local_object->temperature[0]-local_other->temperature[0]) == 0) &&
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

unsigned char *p_trb_event_load(struct o_trb_event *object, unsigned char *raw_data, unsigned char kind, size_t size) {
	unsigned char *result = NULL;
	int index, channel, event_size, event_steps;
	object->filled = d_false;
	if (size >= d_trb_event_size_header)
		if (d_trb_event_header(raw_data)) {
			object->code = raw_data[2];
			object->kind = raw_data[3];
			switch (object->kind) {
				case 0xa0:
				case 0xa1:
					event_steps = d_trb_event_channels;
					event_size = d_trb_event_size_normal;
					break;
				case 0xa3:
					event_steps = d_trb_event_samples;
					event_size = d_trb_event_size_debug;
					break;
			}
			result = (raw_data+4);
			if ((kind == object->kind) && (size >= event_size)) {
				memset(object->values, 0, sizeof(unsigned short int)*d_trb_event_channels);
				for (index = 0; index < event_steps; index++, result += 2) {
					channel = ((index%2)*d_trb_event_channels_half)+(index/2);
					object->values[channel] = ((unsigned short int)result[0])|((unsigned short int)result[1])<<8;
				}
				object->temperature[0] = ((unsigned short int)result[0])|((unsigned short int)result[1])<<8;
				result += 2;
				object->temperature[1] = ((unsigned short int)result[0])|((unsigned short int)result[1])<<8;
				result += 2;
				object->version = ((unsigned short int)result[0])|((unsigned short int)result[1])<<8;
				result += 2;
				object->filled = d_true;
			}
		}
	return result;
}

