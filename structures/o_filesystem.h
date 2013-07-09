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
#ifndef serenity_o_filesystem_h
#define serenity_o_filesystem_h
#include "o_object.h"
#include "o_stream.h"
#include "o_dictionary.h"
#define d_filesystem_name_size 16
extern const char v_filesystem_kind[];
typedef struct s_filesystem_header {
	char name[d_filesystem_name_size];
	ssize_t bytes;
} s_filesystem_header;
typedef struct o_filesystem {
	d_object_head;
	struct o_dictionary *files;
	int (*m_insert)(struct o_filesystem *, struct o_string *,
			struct o_stream *);
	struct o_stream *(*m_get)(struct o_filesystem *, struct o_string *);
	void (*m_store)(struct o_filesystem *, struct o_stream *);
} o_filesystem;
extern void p_filesystem_hooking(struct o_filesystem *object);
extern struct o_filesystem *f_filesystem_new(struct o_filesystem *supplied);
extern struct o_filesystem *f_filesystem_new_stream(struct o_filesystem *supplied,
		struct o_stream *stream);
extern void p_filesystem_delete(struct o_object *object);
extern int p_filesystem_compare(struct o_object *object,
		struct o_object *other);
extern char *p_filesystem_string(struct o_object *object,
		char *data, size_t size);
extern struct o_object *p_filesystem_clone(struct o_object *object);
extern int p_filesystem_insert(struct o_filesystem *object,
		struct o_string *name, struct o_stream *stream);
extern struct o_stream *p_filesystem_get(struct o_filesystem *object,
		struct o_string *name);
extern void p_filesystem_store(struct o_filesystem *object,
		struct o_stream *destination);
#endif
