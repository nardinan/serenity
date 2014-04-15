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
#ifndef serenity_o_trbs_h
#define serenity_o_trbs_h
#include "o_trb.h"
#include <usb.h>
#define d_trbs_slots_size 15
extern const char v_trbs_kind[];
extern int v_trbs_usb_init;
typedef int (t_trbs_trb_handle)(struct o_trb *, void *);
typedef int (t_trbs_dev_handle)(struct usb_device *, struct usb_dev_handle *, void *);
typedef struct s_trbs_parameters {
	struct o_trbs *object;
	t_trbs_trb_handle *trb_handle;
	t_trbs_dev_handle *dev_handle;
	void *trb_data, *dev_data;
	time_t sleep;
} s_trbs_parameters;
typedef struct o_trbs {
	d_object_head;
	struct o_object *semaphore, *search_semaphore;
	struct s_trbs_parameters *parameters;
	struct {
		struct usb_device *device;
		int referenced;
	} devices[d_trbs_slots_size];
	pthread_t thread_id;
	void (*m_trb_setup)(struct o_trbs *, t_trbs_trb_handle *, void *);
	void (*m_dev_setup)(struct o_trbs *, t_trbs_dev_handle *, void *);
	int (*m_async_search)(struct o_trbs *, time_t);
} o_trbs;
extern void p_trbs_hooking(struct o_trbs *object);
extern int p_trbs_thread_continue(struct o_trbs *object);
extern void *p_trbs_thread(void *parameters);
extern struct o_trbs *f_trbs_new(struct o_trbs *supplied);
extern void p_trbs_delete(struct o_object *object);
extern int p_trbs_compare(struct o_object *object, struct o_object *other);
extern t_hash_value p_trbs_hash(struct o_object *object);
extern char *p_trbs_string(struct o_object *object, char *data, size_t size);
extern void p_trbs_trb_setup(struct o_trbs *object, t_trbs_trb_handle *handle, void *user_data);
extern void p_trbs_dev_setup(struct o_trbs *object, t_trbs_dev_handle *handle, void *user_data);
extern int p_trbs_async_search(struct o_trbs *object, time_t sleep);
#endif
