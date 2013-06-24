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
#include "o_object.h"
void p_object_hooking(struct o_object *object) {
	object->s_delegate.m_compare = p_object_compare;
	object->s_delegate.m_hash = p_object_hash;
	object->s_delegate.m_string = p_object_string;
	object->s_delegate.m_clone = p_object_clone;
	object->s_delegate.m_trylock = p_object_trylock;
	object->s_delegate.m_lock = p_object_lock;
	object->s_delegate.m_unlock = p_object_unlock;
}

struct o_object *f_object_new(const char *kind, size_t size,
							  struct o_object *supplied) {
	struct o_object *result = supplied;
	if (result) {
		memset(result, 0, size);
		result->s_flags.supplied = d_true;
	} else if ((result = (struct o_object *) d_calloc(1, size)))
		result->s_flags.supplied = d_false;
	else
		d_die(d_error_malloc);
	if (pthread_mutex_init(&(result->mutex), NULL) == 0)
		result->s_flags.mutexed = d_true;
	result->kind = kind;
	result->size = size;
	p_object_hooking(result);
	return result;
}

struct o_object *f_object_retain(struct o_object *object) {
	if (object)
		object->references++;
	return object;
}

void f_object_release(struct o_object *object) {
	if (object) {
		if ((object->references == 0) && (!object->s_flags.pooled)) {
			if (object->s_delegate.m_delete)
				object->s_delegate.m_delete(object);
			p_object_delete(object);
		} else
			object->references--;
	}
}

void p_object_delete(struct o_object *object) {
	if (object->s_flags.mutexed)
		pthread_mutex_destroy(&(object->mutex));
	if (!object->s_flags.supplied) {
		d_free(object);
		object = NULL;
	} else
		memset(object, 0, object->size);
}

int p_object_compare(struct o_object *object, struct o_object *other) {
	return (int)(object-other);
}

t_hash_value p_object_hash(struct o_object *object) {
	if (!object->s_flags.hashed) {
		object->hash = (t_hash_value)object;
		object->s_flags.hashed = d_true;
	}
	return object->hash;
}

char *p_object_string(struct o_object *object, char *data, size_t size) {
	size_t written;
	written = snprintf(data, size, "<%s size: %zu>", object->kind,
					   object->size);
	return data+((written>size)?size:written);
}

struct o_object *p_object_clone(struct o_object *object) {
	struct o_object *result;
	if ((result = f_object_new(object->kind, object->size, NULL))) {
		memcpy(result, object, object->size);
		if (pthread_mutex_init(&(result->mutex), NULL) == 0)
			result->s_flags.mutexed = d_true;
		result->references = 0;
	}
	return result;
}

int p_object_trylock(struct o_object *object) {
	int locked = d_false;
	if (object->s_flags.mutexed)
		if (pthread_mutex_trylock(&(object->mutex)) == 0)
			locked = d_true;
	return locked;
}

void p_object_lock(struct o_object *object) {
	if (object->s_flags.mutexed)
		pthread_mutex_lock(&(object->mutex));
}

void p_object_unlock(struct o_object *object) {
	if (object->s_flags.mutexed)
		pthread_mutex_unlock(&(object->mutex));
}
