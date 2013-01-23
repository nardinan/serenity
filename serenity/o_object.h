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
#ifndef serenity_object_h
#define serenity_object_h
#include "../ground/ground.h"
#define d_object(n) typedef struct n
#define d_object_head struct o_object head
#define d_retain(o,k) (k *)f_object_retain((struct o_object *)o)
#define d_release(o) f_object_release((struct o_object *)o)
#define d_clone(o,k) (k *)o->head.s_delegate.m_clone((struct o_object *)o)
typedef struct o_object {
    const char *kind;
	size_t size, references;
	t_hash_value hash;
	struct {
		unsigned int supplied:1;
		unsigned int hashed:1;
	} s_flags;
	struct {
		void (*m_delete)(struct o_object *);
		int (*m_compare)(struct o_object *, struct o_object *);
		t_hash_value (*m_hash)(struct o_object *);
		char *(*m_string)(struct o_object *, char *, size_t);
		struct o_object *(*m_clone)(struct o_object *);
	} s_delegate;
} o_object;
extern void p_object_hooking(struct o_object *object);
extern struct o_object *f_object_new(const char *kind, size_t size, struct o_object *supplied);
extern struct o_object *f_object_retain(struct o_object *object);
extern void f_object_release(struct o_object *object);
extern void p_object_delete(struct o_object *object);
extern int p_object_compare(struct o_object *object, struct o_object *other);
extern t_hash_value p_object_hash(struct o_object *object);
extern char *p_object_string(struct o_object *object, char *data, size_t size);
extern struct o_object *p_object_clone(struct o_object *object);
#endif
