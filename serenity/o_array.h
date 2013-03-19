/*
     serenity
     Copyright (C) 2013 Andrea Nardinocchi (andrea@nardinan.it)
     
     This program is free software: you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation, either version 3 of the License, or
     (at your option) any later version.
     
     This program is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     GNU General Public License for more details.
     
     You should have received a copy of the GNU General Public License
     along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef serenity_o_hash_h
#define serenity_o_hash_h
#include "o_object.h"
extern const char v_array_kind[];
typedef struct o_array {
	d_object_head;
	struct o_object **content;
	size_t size, filled;
	size_t (*m_insert)(struct o_array *, struct o_object *, size_t);
	struct o_object *(*m_obtain)(struct o_array *, size_t);
} o_array;
extern void p_array_hooking(struct o_array *object);
extern struct o_array *f_array_new(struct o_array *supplied, size_t size);
extern struct o_array *f_array_new_list(struct o_array *supplied,
										size_t size, ...);
extern void p_array_delete(struct o_object *object);
extern int p_array_compare(struct o_object *object, struct o_object *other);
extern char *p_array_string(struct o_object *object, char *data, size_t size);
extern struct o_object *p_array_clone(struct o_object *object);
extern size_t p_array_insert(struct o_array *object, struct o_object *value,
							 size_t position);
extern struct o_object *p_array_obtain(struct o_array *object, size_t position);
#endif
