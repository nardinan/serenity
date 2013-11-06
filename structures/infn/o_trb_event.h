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
#ifndef serenity_o_trb_event_h
#define serenity_o_trb_event_h
#include <math.h>
#include "../o_array.h"
#define d_trb_event_channels 384
#define d_trb_event_channels_half 192
#define d_trb_event_vas 6
#define d_trb_event_channels_on_va 64
#define d_trb_event_size_normal 780
#define d_trb_event_size_debug 112
#define d_trb_event_size_minimum 112
#define d_trb_event_header(v) (((v)[0]==0x90)&&((v)[1]==0xeb))
extern const char v_trb_event_kind[];
typedef struct o_trb_event {
	d_object_head;
	unsigned char code, kind;
	unsigned short int values[d_trb_event_channels], temperature[2];
	int filled;
	unsigned char *(*m_load)(struct o_trb_event *, unsigned char *, size_t);
} o_trb_event;
extern void p_trb_event_hooking(struct o_trb_event *object);
extern unsigned int p_trb_event_align(unsigned char *buffer, size_t size);
extern float *p_trb_event_pedestal(struct o_array *events, float *supplied);
extern float *p_trb_event_sigma_raw(struct o_array *events, float *supplied);
extern float *p_trb_event_sigma(struct o_array *events, float sigma_multiplicator, float *sigma_raw, float *pedestal, float *supplied);
extern struct o_trb_event *f_trb_event_new(struct o_trb_event *supplied);
extern int p_trb_event_compare(struct o_object *object, struct o_object *other);
extern t_hash_value p_trb_event_hash(struct o_object *object);
extern char *p_trb_event_string(struct o_object *object, char *data, size_t size);
extern unsigned char *p_trb_event_load(struct o_trb_event *object, unsigned char *raw_data, size_t size);
#endif

