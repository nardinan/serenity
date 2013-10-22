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
#ifndef serenity_o_trb_h
#define serenity_o_trb_h
#include "../o_string.h"
#include <usb.h>
#define d_trb_manufacturer_label "DAMPE"
#define d_trb_product_label "miniTRB-Device"
#define d_trb_write_endpoint 0
#define d_trb_read_endpoint 1
#define d_trb_channels 384
#define d_trb_buffer_size 1560
#define d_trb_event_size_normal 780
#define d_trb_event_size_debug 112
extern const char v_trb_kind[];
enum e_trb_mode {
	e_trb_mode_normal,
	e_trb_mode_calibration,
	e_trb_mode_calibration_debug_analogic,
	e_trb_mode_calibration_debug_digital
} e_trb_mode;
typedef struct s_event {
	unsigned char code;
	short unsigned int values[d_trb_channels];
	int filled;
} s_event;
typedef struct o_trb {
	d_object_head;
	struct usb_device *device;
	struct usb_dev_handle *handler;
	struct o_stream *stream_out;
	int write_address, read_address, buffer_fill, event_size;
	unsigned char buffer[d_trb_buffer_size], mode;
	int (*m_setup)(struct o_trb *, unsigned char, float, enum e_trb_mode, unsigned char, unsigned char, time_t);
	struct s_event *(*m_event)(struct o_trb *, struct s_event *, time_t);
} o_trb;
extern void p_trb_hooking(struct o_trb *object);
extern int p_trb_read(struct o_trb *object, char *data, size_t size, time_t timeout);
extern int p_trb_write(struct o_trb *object, char *data, size_t size, time_t timeout);
extern struct o_trb *f_trb_new(struct o_trb *supplied, struct usb_device *device, struct usb_dev_handle *handler);
extern void p_trb_delete(struct o_object *object);
extern int p_trb_compare(struct o_object *object, struct o_object *other);
extern t_hash_value p_trb_hash(struct o_object *object);
extern char *p_trb_string(struct o_object *object, char *data, size_t size);
extern int p_trb_setup(struct o_trb *object, unsigned char trigger, float hold_delay, enum e_trb_mode mode, unsigned char dac, unsigned char channel,
		time_t timeout);
extern struct s_event *p_trb_event(struct o_trb *object, struct s_event *provided, time_t timeout);
extern void p_trb_close(struct o_trb *object);
#endif
