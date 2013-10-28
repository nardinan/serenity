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
#ifndef serenity_object_h
#define serenity_object_h
#include <pthread.h>
#include "o_exceptions.h"
#define d_object_head struct o_object head
#define d_compare(a,b) (((a)&&(b))?((struct o_object *)(a))->s_delegate.m_compare((struct o_object*)(a),(struct o_object*)(b)):(int)((a)-(b)))
#define d_retain(a,k) (k*)f_object_retain((struct o_object *)a)
#define d_release(a) f_object_release((struct o_object *)a)
#define d_clone(a,k) (k*)((struct o_object *)(a))->s_delegate.m_clone((struct o_object *)a)
#define d_object_constant(k,s,hook)\
{\
	d_list_node_constant,\
	.kind = (k),\
	.size = (s),\
	0,\
	0,\
	0,\
	.s_flags = {\
		d_true,\
		d_false,\
		d_false,\
		d_false\
	},\
	.s_delegate = {\
		p_##hook##_delete,\
		p_##hook##_compare,\
		p_##hook##_hash,\
		p_##hook##_string,\
		p_##hook##_clone,\
		p_object_trylock,\
		p_object_lock,\
		p_object_unlock\
	}\
}
#define d_object_kind(o,k) ((struct o_##k *)(((o)->kind==v_##k##_kind)?(o):NULL))
#define d_object_kind_compare(a,b) ((a)->kind==(b)->kind)
#define d_object_trylock(a) ((a)->s_delegate.m_trylock(a))
#define d_object_lock(a) ((a)->s_delegate.m_lock(a))
#define d_object_unlock(a) ((a)->s_delegate.m_unlock(a))
typedef struct o_object {
	d_list_node_head;
	const char *kind;
	size_t size, references;
	t_hash_value hash;
	pthread_mutex_t mutex;
	struct {
		unsigned int supplied:1, hashed:1, pooled:1, mutexed:1;
	} s_flags;
	struct {
		void (*m_delete)(struct o_object *);
		int (*m_compare)(struct o_object *, struct o_object *);
		t_hash_value (*m_hash)(struct o_object *);
		char *(*m_string)(struct o_object *, char *, size_t);
		struct o_object *(*m_clone)(struct o_object *);
		int (*m_trylock)(struct o_object *);
		void (*m_lock)(struct o_object *);
		void (*m_unlock)(struct o_object *);
	} s_delegate;
} o_object;
extern void p_object_hooking(struct o_object *object);
extern struct o_object *f_object_new_pure(struct o_object *supplied);
extern struct o_object *f_object_new(const char *kind, size_t size, struct o_object *supplied);
extern struct o_object *f_object_retain(struct o_object *object);
extern void f_object_release(struct o_object *object);
extern void p_object_delete(struct o_object *object);
extern int p_object_compare(struct o_object *object, struct o_object *other);
extern t_hash_value p_object_hash(struct o_object *object);
extern char *p_object_string(struct o_object *object, char *data, size_t size);
extern struct o_object *p_object_clone(struct o_object *object);
extern int p_object_trylock(struct o_object *object);
extern void p_object_lock(struct o_object *object);
extern void p_object_unlock(struct o_object *object);
#endif

