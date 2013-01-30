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
#ifndef serenity_o_pool_h
#define serenity_o_pool_h
#include "o_object.h"
#define d_pool_begin(p)\
{\
	struct o_pool *__d_pool=(p);\
	do
#define d_pool_end\
	while(0);\
}
#define d_P(a,k) (k*)__d_pool->m_insert(__d_pool,(struct o_object *)(a))
#define d_A(a,k) d_retain(P(__d_pool,a,struct o_object),k)
extern const char v_pool_kind[];
typedef struct o_pool {
	d_object_head;
	s_list *pool;
	struct o_object *(*m_insert)(struct o_pool *, struct o_object *);
	void (*m_clean)(struct o_pool *, int);
} o_pool;
extern void p_pool_hooking(struct o_pool *object);
extern struct o_pool *f_pool_new(struct o_pool *supplied);
extern void p_pool_delete(struct o_object *object);
extern char *p_pool_string(struct o_object *object, char *data, size_t size);
extern struct o_object *p_pool_clone(struct o_object *object);
extern struct o_object *p_pool_insert(struct o_pool *object,
									  struct o_object *value);
extern void p_pool_clean(struct o_pool *object, int bypass);
#endif
