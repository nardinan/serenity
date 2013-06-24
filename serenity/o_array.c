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
#include "o_array.h"
const char v_array_kind[] = "array";
void p_array_hooking(struct o_array *object) {
	object->head.s_delegate.m_delete = p_array_delete;
	object->head.s_delegate.m_compare = p_array_compare;
	/* keep default hash function */
	object->head.s_delegate.m_string = p_array_string;
	object->head.s_delegate.m_clone = p_array_clone;
	object->m_insert = p_array_insert;
	object->m_remove = p_array_remove;
	object->m_get = p_array_get;
}

struct o_array *f_array_new(struct o_array *supplied, size_t size) {
	return f_array_new_bucket(supplied, d_array_default_bucket, size);
}

struct o_array *f_array_new_bucket(struct o_array *supplied, size_t bucket,
								   size_t size) {
	struct o_array *result;
	if ((result = (struct o_array *)
		 f_object_new(v_array_kind, sizeof(struct o_array),
					  (struct o_object *)supplied))) {
		p_array_hooking(result);
		if ((result->content = (struct o_object **)
			d_calloc(size, sizeof(struct o_object *)))) {
			result->size = size;
			result->bucket = bucket;
		} else
			d_die(d_error_malloc);
	}
	return result;
}

struct o_array *f_array_new_list(struct o_array *supplied, size_t bucket,
								 size_t size, ...) {
	va_list arguments;
	struct o_array *result;
	struct o_object *value;
	if ((result = (struct o_array *)
		 f_object_new(v_array_kind, sizeof(struct o_array),
					  (struct o_object *)supplied))) {
		p_array_hooking(result);
		if ((result->content = (struct o_object **)
			 d_calloc(size, sizeof(struct o_object *)))) {
			result->size = size;
			result->bucket = bucket;
			va_start(arguments, size);
			while ((result->filled < result->size) &&
				   (value = va_arg(arguments, struct o_object *)))
				result->content[result->filled++] = d_retain(value,
															 struct o_object);
			va_end(arguments);
		} else
			d_die(d_error_malloc);
	}
	return result;
}

void p_array_delete(struct o_object *object) {
	struct o_array *local_object;
	size_t index;
	if ((local_object = d_object_kind(object, array))) {
		for (index = 0; index < local_object->size; index++) {
			d_release(local_object->content[index]);
			local_object->content[index] = NULL;
		}
		d_free(local_object->content);
	} else
		d_throw(v_exception_kind, "object is not an instance of o_array");
}

int p_array_compare(struct o_object *object, struct o_object *other) {
	struct o_array *local_object, *local_other;
	int result = p_object_compare(object, other), index;
	if ((local_object = d_object_kind(object, array)) &&
		(local_other = d_object_kind(other, array))) {
		result = (int)(local_object->filled-local_other->filled);
		for (index = 0; (result == 0)&&(index < local_object->size); index++)
			result = d_compare(local_object->content[index],
							   local_other->content[index]);
	}
	return result;
}

char *p_array_string(struct o_object *object, char *data, size_t size) {
	struct o_array *local_object;
	struct o_object *value;
	char *pointer = data, *next;
	size_t index, written = 0;
	if ((local_object = d_object_kind(object, array))) {
		if (written < size) {
			*pointer = '[';
			if ((++written) < size)
				pointer++;
		}
		for (index = 0; index < local_object->size; index++)
			if ((value = local_object->content[index])) {
				next = value->s_delegate.m_string(local_object->content[index],
												  pointer, (size-written));
				written += (next-pointer);
				pointer = next;
				if ((written < size) && ((index+1) < local_object->size)) {
					*pointer = ',';
					if ((++written) < size)
						pointer++;
				}
			}
		if (written < size) {
			*pointer = ']';
			if ((++written) < size)
				pointer++;
		}
		*pointer = '\0';
	} else
		d_throw(v_exception_kind, "object is not an instance of o_array");
	return pointer;
}

struct o_object *p_array_clone(struct o_object *object) {
	struct o_array *result = NULL, *local_object;
	size_t index;
	if ((local_object = d_object_kind(object, array))) {
		result = (struct o_array *)p_object_clone(object);
		if (local_object->size > 0) {
			if ((result->content = (struct o_object **)
				 d_calloc(local_object->size, sizeof(struct o_object *)))) {
				result->size = local_object->size;
				result->bucket = local_object->bucket;
				for (index = 0; index < local_object->size; index++)
					if (local_object->content[index])
						result->content[index] =
						d_retain(local_object->content[index], struct o_object);
				result->filled = local_object->filled;
			}
		}
	} else
		d_throw(v_exception_kind, "object is not an instance of o_array");
	return (o_object *)result;
}

size_t p_array_insert(struct o_array *object, struct o_object *value,
					  size_t position) {
	if (position <= object->size) {
		if (position == object->size) {
			if ((object->content = (struct o_object **)
				 d_realloc(object->content, ((object->size+object->bucket)*
											 sizeof(struct o_object *))))) {
				memset(&(object->content[object->size]), 0,
					   (object->bucket*sizeof(struct o_object *)));
				object->size += object->bucket;
			} else
				d_die(d_error_malloc);
		} else if (object->content[position]) {
			if (object->content[object->size-1]) {
				if ((object->content = (struct o_object **)
					 d_realloc(object->content, ((object->size+object->bucket)*
												 sizeof(struct o_object *))))) {
					memset(&(object->content[object->size]), 0,
						   (object->bucket*sizeof(struct o_object *)));
					memmove(&(object->content[position+1]),
							&(object->content[position]),
							((object->size-position)*
							 sizeof(struct o_object *)));
					object->size += object->bucket;
				} else
					d_die(d_error_malloc);
			} else
				memmove(&(object->content[position+1]),
						&(object->content[position]),
						(object->size-position-1)*sizeof(struct o_object *));
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

int p_array_remove(struct o_array *object, size_t position) {
	int result = d_false;
	if (position < object->size) {
		if (object->content[position]) {
			d_release(object->content[position]);
			object->content[position] = NULL;
			object->filled--;
			result = d_true;
		}
	} else
		d_throw(v_exception_bound, "index is over the array's size");
	return result;
}

struct o_object *p_array_get(struct o_array *object, size_t position) {
	struct o_object *result = NULL;
	if (position < object->size)
		result = object->content[position];
	else
		d_throw(v_exception_bound, "index is over the array's size");
	return result;
}
