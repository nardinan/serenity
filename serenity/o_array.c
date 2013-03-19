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
#include "o_array.h"
const char v_array_kind[] = "array";
void p_array_hooking(struct o_array *object) {
	object->head.s_delegate.m_delete = p_array_delete;
	object->head.s_delegate.m_compare = p_array_compare;
	/* keep default hash function */
	object->head.s_delegate.m_string = p_array_string;
	object->head.s_delegate.m_clone = p_array_clone;
	object->m_insert = p_array_insert;
	object->m_obtain = p_array_obtain;
}

struct o_array *f_array_new(struct o_array *supplied, size_t size) {
	struct o_array *result;
	if ((result = (struct o_array *)
		 f_object_new(v_array_kind, sizeof(struct o_array),
					  (struct o_object *)supplied))) {
		if ((result->content = (struct o_object **)
			 calloc(size, sizeof(struct o_object *)))) {
			result->size = size;
			p_array_hooking(result);
		} else
			d_die(d_error_malloc);
	} else
		d_throw(v_exception_kind, "object is not an instance of o_array");
	return result;
}

struct o_array *f_array_new_list(struct o_array *supplied, size_t size, ...) {
	va_list arguments;
	struct o_array *result;
	struct o_object *value;
	if ((result = (struct o_array *)
		 f_object_new(v_array_kind, sizeof(struct o_array),
					  (struct o_object *)supplied))) {
		 if ((result->content = (struct o_object **)
			  calloc(size, sizeof(struct o_object *)))) {
			 result->size = size;
			 va_start(arguments, size);
			 while ((result->filled < result->size) &&
					(value = va_arg(arguments, struct o_object *)))
				 result->content[result->filled++] = d_retain(value,
															  struct o_object);
			 va_end(arguments);
			 p_array_hooking(result);
		 } else
			 d_die(d_error_malloc);
	 } else
		 d_throw(v_exception_kind, "object is not an instance of o_array");
	return result;
}

void p_array_delete(struct o_object *object) {
	struct o_array *local_object;
	size_t index;
	if (d_object_kind(object, v_array_kind)) {
		local_object = (struct o_array *)object;
		for (index = 0; index < local_object->size; index++) {
			d_release(local_object->content[index]);
			local_object->content[index] = NULL;
		}
		free(local_object->content);
	} else
		d_throw(v_exception_kind, "object is not an instance of o_array");
}

int p_array_compare(struct o_object *object, struct o_object *other) {
	struct o_array *local_object, *local_other;
	int result = p_object_compare(object, other), index;
	if ((d_object_kind(object, v_array_kind)) &&
		(d_object_kind(other, v_array_kind))) {
		local_object = (struct o_array *)object;
		local_other = (struct o_array *)other;
		result = (int)(local_object->filled-local_other->filled);
		for (index = 0; (result == 0)&&(index < local_object->size); index++)
			result = d_compare(local_object->content[index],
							   local_other->content[index]);
	}
	return result;
}

char *p_array_string(struct o_object *object, char *data, size_t size) {
	char *pointer = data, *next;
	struct o_array *local_object;
	struct o_object *value;
	size_t index;
	if (d_object_kind(object, v_array_kind)) {
		local_object = (struct o_array *)object;
		for (index = 0; index < local_object->size; index++)
			if ((value = local_object->content[index])) {
				next = value->s_delegate.m_string(local_object->content[index],
												  pointer, size);
				if ((size -= (next-pointer)) > 0) {
					*next = ',';
					next++;
					size--;
				}
				pointer = next;
			}
	} else
		d_throw(v_exception_kind, "object is not an instance of o_array");
	return pointer;
}

struct o_object *p_array_clone(struct o_object *object) {
	struct o_array *result = NULL, *local_object;
	size_t index;
	if (d_object_kind(object, v_array_kind)) {
		result = (struct o_array *)p_object_clone(object);
		local_object = (struct o_array *)object;
		if (local_object->size > 0) {
			result->filled = local_object->filled;
			if ((result->content = (struct o_object **)
				 calloc(1, local_object->size)))
				for (index = 0; index < local_object->size; index++)
					if (local_object->content[index])
						result->content[index] =
						d_retain(local_object->content[index], struct o_object);
		}
	} else
		d_throw(v_exception_kind, "object is not an instance of o_array");
	return (o_object *)result;
}

size_t p_array_insert(struct o_array *object, struct o_object *value,
					  size_t position) {
	if (position < object->size) {
		if (object->content[position]) {
			d_release(object->content[position]);
			object->filled--;
		}
		if (value) {
			object->content[position] = d_retain(value, struct o_object);
			object->filled++;
		} else
			object->content[position] = NULL;
	} else
		d_throw(v_exception_bound, "index is over the array's size");
	return position;
}

struct o_object *p_array_obtain(struct o_array *object, size_t position) {
	struct o_object *result = NULL;
	if (position < object->size)
		result = object->content[position];
	else
		d_throw(v_exception_bound, "index is over the array's size");
	return result;
}
