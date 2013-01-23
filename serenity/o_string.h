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
#ifndef serenity_o_string_h
#define serenity_o_string_h
#include "o_object.h"
#define d_string_buffer_size 256
#define d_string(c) f_string_new(NULL,d_true,d_strlen(c),(c))
extern const char v_string_kind[];
d_object(o_string) {
	d_object_head;
	struct {
		unsigned int constant:1;
	} s_flags;
	char *content;
	size_t size;
	void (*m_trim)(struct o_string *);
	void (*m_append)(struct o_string *, struct o_string *);
} o_string;
extern void p_string_hooking(struct o_string *object);
extern struct o_string *f_string_new(struct o_string *supplied, int constant, size_t size, const char *content);
extern struct o_string *f_string_new_format(struct o_string *supplied, int constant, size_t size, const char *format, ...);
extern char *p_string_format_object_kind(char *buffer, size_t size, char *format, va_list parameters);
extern char *p_string_format_object_content(char *buffer, size_t size, char *format, va_list parameters);
extern void p_string_delete(struct o_object *object);
extern int p_string_compare(struct o_object *object, struct o_object *other);
extern t_hash_value p_string_hash(struct o_object *object);
extern char *p_string_string(struct o_object *object, char *data, size_t size);
extern struct o_object *p_string_clone(struct o_object *object);
extern void p_string_trim(struct o_string *object);
extern void p_string_append(struct o_string *object, struct o_string *other);
#endif
