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
#ifndef serenity_o_dictionary_h
#define serenity_o_dictionary_h
#include "o_object.h"
#include "o_stream.h"
#include "o_array.h"
#define d_dictionary_get_string(d,k) (struct o_string *)((d)->m_get((d),(struct o_object *)(k)))
extern const char v_dictionary_kind[];
typedef struct o_dictionary {
	d_object_head;
	struct s_hash_table *table;
	int (*m_load)(struct o_dictionary *, struct o_stream *);
	int (*m_insert)(struct o_dictionary *, struct o_object *, struct o_object *);
	struct o_object *(*m_get)(struct o_dictionary *, struct o_object *);
	struct o_array *(*m_keys)(struct o_dictionary *);
	struct o_array *(*m_values)(struct o_dictionary *);
} o_dictionary;
extern void p_dictionary_hooking(struct o_dictionary *object);
extern t_hash_value p_dictionary_hash_hooker(void *key);
extern int p_dictionary_compare_hooker(void *left, void *right);
extern struct o_dictionary *f_dictionary_new(struct o_dictionary *supplied);
extern void p_dictionary_delete(struct o_object *object);
extern char *p_dictionary_string(struct o_object *object, char *data, size_t size);
extern struct o_object *p_dictionary_clone(struct o_object *object);
extern int p_dictionary_load(struct o_dictionary *object, struct o_stream *stream);
extern int p_dictionary_insert(struct o_dictionary *object, struct o_object *key, struct o_object *value);
extern struct o_object *p_dictionary_get(struct o_dictionary *object, struct o_object *key);
extern struct o_array *p_dictionary_keys(struct o_dictionary *object);
extern struct o_array *p_dictionary_values(struct o_dictionary *object);
#endif

