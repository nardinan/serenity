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
#include "o_dictionary.h"
#include "o_string.h"
const char v_dictionary_kind[] = "dictionary";
void p_dictionary_hooking(struct o_dictionary *object) {
	object->head.s_delegate.m_delete = p_dictionary_delete;
	/* keep default compare function */
	/* keep default hash function */
	object->head.s_delegate.m_string = p_dictionary_string;
	object->head.s_delegate.m_clone = p_dictionary_clone;
	object->m_insert = p_dictionary_insert;
	object->m_get = p_dictionary_get;
	object->m_keys = p_dictionary_keys;
	object->m_values = p_dictionary_values;
}

extern t_hash_value p_dictionary_hash_hooker(void *key) {
	struct o_object *object = (struct o_object *)key;
	t_hash_value val = object->s_delegate.m_hash(object);
	return val;
}

extern int p_dictionary_compare_hooker(void *left, void *right) {
	struct o_object *object_left = (struct o_object *)left,
					*object_right = (struct o_object *)right;
	return object_left->s_delegate.m_compare(object_left, object_right);
}

struct o_dictionary *f_dictionary_new(struct o_dictionary *supplied) {
	struct o_dictionary *result;
	if ((result = (struct o_dictionary *)
		 f_object_new(v_dictionary_kind, sizeof(struct o_dictionary),
					  (struct o_object *)supplied))) {
		p_dictionary_hooking(result);
		f_hash_init(&(result->table), p_dictionary_compare_hooker,
					p_dictionary_hash_hooker);
	}
	return result;
}

void p_dictionary_delete(struct o_object *object) {
	struct o_dictionary *local_object;
	struct o_array *keys;
	struct o_object *key;
	struct s_hash_bucket current;
	size_t index;
	if ((local_object = d_object_kind(object, dictionary))) {
		if ((keys = local_object->m_keys(local_object))) {
			for (index = 0; index < keys->size; index++)
				if ((key = keys->m_get(keys, index))) {
					f_hash_delete(local_object->table, key, &current);
					if (current.kind != e_hash_kind_empty) {
						if (current.key)
							d_release(current.key);
						if (current.value)
							d_release(current.value);
						memset(&current, 0, sizeof(struct s_hash_bucket));
					}
				}
			d_release(keys);
		}
		f_hash_destroy(&(local_object->table));
	} else
		d_throw(v_exception_kind, "object is not an instance of o_dictionary");
}

char *p_dictionary_string(struct o_object *object, char *data, size_t size) {
	struct o_dictionary *local_object;
	struct o_array *keys;
	struct o_object *key, *value;
	char *pointer = data, *next;
	size_t index, written = 0;
	if ((local_object = d_object_kind(object, dictionary))) {
		if (written < size) {
			*pointer = '{';
			if ((++written) < size)
				pointer++;
		}
		if ((keys = local_object->m_keys(local_object))) {
			for (index = 0; index < keys->size; index++) {
				if ((key = keys->m_get(keys, index))) {
					next = key->s_delegate.m_string(key, pointer,
													(size-written));
					written += (next-pointer);
					pointer = next;
					if (written < size) {
						*pointer = ':';
						if ((++written) < size) {
							pointer++;
							if ((value =
								 f_hash_get(local_object->table, key))) {
								next =
								value->s_delegate.m_string(value, pointer,
														   (size-written));
								written += (next-pointer);
								pointer = next;
								if (written < size) {
									*pointer = ',';
									if ((++written) < size)
										pointer++;
								}
							}
						}
					}
				}
			}
			d_release(keys);
		}
		if (written < size) {
			*pointer = '}';
			if ((++written) < size)
				pointer++;
		}
		*pointer = '\0';
	} else
		d_throw(v_exception_kind, "object is not an instance of o_dictionary");
	return pointer;
}

struct o_object *p_dictionary_clone(struct o_object *object) {
	struct o_dictionary *result = NULL, *local_object;
	struct o_array *keys;
	struct o_object *key;
	size_t index;
	if ((local_object = d_object_kind(object, dictionary))) {
		result = (struct o_dictionary *)p_object_clone(object);
		result->table = NULL;
		f_hash_init(&(result->table), p_dictionary_compare_hooker,
					p_dictionary_hash_hooker);
		if ((keys = local_object->m_keys(local_object))) {
			for (index = 0; index < keys->size; index++) {
				key = keys->m_get(keys, index);
				result->m_insert(result, key,
								 f_hash_get(local_object->table, key));
			}
			d_release(keys);
		}
	} else
		d_throw(v_exception_kind, "object is not an instance of o_dictionary");
	return (o_object *)result;
}

int p_dictionary_insert(struct o_dictionary *object, struct o_object *key,
						struct o_object *value) {
	struct s_hash_bucket replaced = {0};
	int result;
	result = f_hash_insert(object->table, d_retain(key, void),
						   d_retain(value, void), s_true, &replaced);
	if (replaced.value) {
		/* an old value has been replaced */
		d_release(key);
		d_release((struct o_object *)replaced.value);
	}
	return result;
}

struct o_object *p_dictionary_get(struct o_dictionary *object,
								  struct o_object *key) {
	return (struct o_object *)f_hash_get(object->table, (void *)key);
}

struct o_array *p_dictionary_keys(struct o_dictionary *object) {
	struct o_array *result = f_array_new(NULL, object->table->fill);
	struct s_hash_bucket *singleton;
	size_t index = 0, position = 0, elements = (object->table->mask+1);
	for (index = 0; index < elements; index++) {
		singleton = &(object->table->table[index]);
		if (singleton->kind == e_hash_kind_fill)
			result->m_insert(result, singleton->key, position++);
	}
	return result;
}

struct o_array *p_dictionary_values(struct o_dictionary *object) {
	struct o_array *result = f_array_new(NULL, object->table->fill);
	struct s_hash_bucket *singleton;
	size_t index = 0, position = 0, elements = (object->table->mask+1);
	for (index = 0; index < elements; index++) {
		singleton = &(object->table->table[index]);
		if (singleton->kind == e_hash_kind_fill)
			result->m_insert(result, singleton->value, position++);
	}
	return result;
}
