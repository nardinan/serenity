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
#include "o_pool.h"
const char v_pool_kind[] = "pool";
void p_pool_hooking(struct o_pool *object) {
	object->head.s_delegate.m_delete = p_pool_delete;
	/* keep default compare function */
	/* keep default hash function */
	object->head.s_delegate.m_string = p_pool_string;
	object->head.s_delegate.m_clone = p_pool_clone;
	object->m_insert = p_pool_insert;
	object->m_clean = p_pool_clean;
}

struct o_pool *f_pool_new(struct o_pool *supplied) {
	struct o_pool *result;
	if ((result = (struct o_pool *)
		 f_object_new(v_pool_kind, sizeof(struct o_pool),
					  (struct o_object *)supplied)))
		f_list_init(&result->pool);
	p_pool_hooking(result);
	return result;
}

void p_pool_delete(struct o_object *object) {
	struct o_pool *local_object;
	if ((local_object = d_object_kind(object, pool))) {
		if (local_object->pool)
			f_list_destroy(&(local_object->pool));
	} else
		d_throw(v_exception_kind, "object is not an instance of o_pool");
}

char *p_pool_string(struct o_object *object, char *data, size_t size) {
	struct o_pool *local_object;
	struct o_object *value;
	char *pointer = data, *next;
	if ((local_object = d_object_kind(object, pool))) {
		d_foreach(local_object->pool, value, struct o_object) {
			next = value->s_delegate.m_string(value, pointer, size);
			size -= (next-pointer);
			if ((size > 0) && (((struct s_list_node *)value)->next)) {
				*next = ',';
				next++;
				size--;
			}
			pointer = next;
		}
	} else
		d_throw(v_exception_kind, "object is not an instance of o_pool");
	return pointer;
}

struct o_object *p_pool_clone(struct o_object *object) {
	struct o_pool *result = NULL, *local_object;
	struct o_object *value;
	if ((local_object = d_object_kind(object, pool))) {
		result = (struct o_pool *)p_object_clone(object);
		if (local_object->pool) {
			f_list_init(&result->pool);
			d_foreach(local_object->pool, value, struct o_object)
				f_list_append(result->pool,
							  (struct s_list_node *)d_retain(value,
															 struct o_object),
							  e_list_insert_head);
		}
	} else
		d_throw(v_exception_kind, "object is not an instance of o_pool");
	return (struct o_object *)result;
}

struct o_object *p_pool_insert(struct o_pool *object, struct o_object *value) {
	if (object->pool) {
		f_list_append(object->pool, (struct s_list_node *)value,
					  e_list_insert_head);
		value->s_flags.pooled = d_true;
	}
	return value;
}

void p_pool_clean(struct o_pool *object, int bypass) {
	struct o_object *value, *next;
	if (object->pool) {
		value = (struct o_object *)object->pool->head;
		while (value) {
			next = (struct o_object *)value->head.next;
			if ((bypass) || (value->references == 0)) {
				f_list_delete(object->pool, (struct s_list_node *)value);
				if (value->s_delegate.m_delete)
					value->s_delegate.m_delete(value);
				p_object_delete(value);
			} else
				value->references--;
			value = next;
		}
	}
}
