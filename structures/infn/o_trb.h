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
#include "../o_stream.h"
#include "o_trb_event.h"
#include <usb.h>
#define d_trb_manufacturer_label "DAMPE"
#define d_trb_product_label "miniTRB-Device"
#define d_trb_write_endpoint 0x02
#define d_trb_read_endpoint 0x86
#define d_trb_packet_size 512
#define d_trb_buffer_size 2048
extern const char v_trb_kind[];
enum e_trb_mode {
	e_trb_mode_normal,
	e_trb_mode_calibration,
	e_trb_mode_calibration_debug_digital
} e_trb_mode;
typedef struct o_trb {
	d_object_head;
	struct usb_device *device;
	struct usb_dev_handle *handler;
	struct o_stream *stream_out;
	struct o_object *stream_lock;
	int write_address, read_address, buffer_fill, event_size, last_error;
	unsigned char buffer[d_trb_buffer_size], kind;
	int (*m_setup)(struct o_trb *, unsigned char, float, enum e_trb_mode, unsigned char, unsigned char, time_t);
	int (*m_stop)(struct o_trb *, time_t);
	void (*m_close_stream)(struct o_trb *);
	void (*m_stream)(struct o_trb *, struct o_stream *, struct o_string *, const char *, int);
	struct o_trb_event *(*m_event)(struct o_trb *, struct o_trb_event *, time_t);
} o_trb;
extern void p_trb_hooking(struct o_trb *object);
extern int p_trb_read(struct o_trb *object, unsigned char *data, size_t size, time_t timeout);
extern int p_trb_write(struct o_trb *object, unsigned char *data, size_t size, time_t timeout);
extern int p_trb_check(struct usb_device *device, struct usb_dev_handle *handler);
extern struct o_trb *f_trb_new(struct o_trb *supplied, struct usb_device *device, struct usb_dev_handle *handler);
extern void p_trb_delete(struct o_object *object);
extern int p_trb_compare(struct o_object *object, struct o_object *other);
extern t_hash_value p_trb_hash(struct o_object *object);
extern char *p_trb_string(struct o_object *object, char *data, size_t size);
extern int p_trb_setup(struct o_trb *object, unsigned char trigger, float hold_delay, enum e_trb_mode mode, unsigned char dac, unsigned char channel,
		time_t timeout);
extern int p_trb_stop(struct o_trb *object, time_t timeout);
extern void p_trb_close_stream(struct o_trb *object);
extern void p_trb_stream(struct o_trb *object, struct o_stream *supplied, struct o_string *name, const char *action, int permission);
extern struct o_trb_event *p_trb_event(struct o_trb *object, struct o_trb_event *provided, time_t timeout);
#endif

