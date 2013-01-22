/*
     serenity
     Copyright (C) 2012 Andrea Nardinocchi (nardinocchi@psychogames.net)
     
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
#include "o_object.h"
struct o_object *f_object_new(const char *kind, size_t size, struct o_object *supplied) {
	struct o_object *result = supplied;
	if (result) {
		memset(result, 0, size);
		result->s_flags.supplied = d_true;
	} else if ((result = (struct o_object *) calloc(1, size)))
		result->s_flags.supplied = d_false;
	else
		d_die(d_error_malloc);
	result->kind = kind;
	result->size = size;
	/* initialize with default methods */
	result->s_delegate.m_compare = p_object_compare;
	result->s_delegate.m_hash = p_object_hash;
	result->s_delegate.m_string = p_object_string;
	result->s_delegate.m_clone = p_object_clone;
	return result;
}

struct o_object *f_object_retain(struct o_object *object) {
	object->references++;
	return object;
}

void f_object_release(struct o_object *object) {
	if (object->references == 0) {
		p_object_delete(object);
		object = NULL;
	} else
		object->references--;
}

void p_object_delete(struct o_object *object) {
	if (object->s_delegate.m_delete)
		object->s_delegate.m_delete(object);
	memset(object, 0, object->size);
	if (object->s_flags.supplied == d_false)
		free(object);
}

int p_object_compare(struct o_object *object, struct o_object *other) {
	return (int)(object-other);
}

t_hash_value p_object_hash(struct o_object *object) {
	return (t_hash_value)object;
}

char *p_object_string(struct o_object *object, char *data, int size) {
	snprintf(data, size, "<%s>", object->kind);
	return data;
}

struct o_object *p_object_clone(struct o_object *object) {
	struct o_object *result;
	if ((result = f_object_new(object->kind, object->size, NULL)))
		result->s_delegate = object->s_delegate;
	else
		d_die(d_error_malloc);
	return result;
}
