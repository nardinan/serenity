/*
	 serenity
	 Copyright (C) 2012 Andrea Nardinocchi (andrea@nardinan.it)
	 
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
#include "o_array.h"
#define d_string_buffer_size 256
#define d_string_hooking_constant \
{\
	p_string_delete,\
	p_string_compare,\
	p_string_hash,\
	p_string_string,\
	p_string_clone\
}
#define d_string_constant(c)\
{\
	d_object_constant(v_string_kind,sizeof(struct o_string),\
						d_string_hooking_constant),\
	{\
		d_true\
	},\
	c,\
	0,\
	d_strlen(c),\
	p_string_trim,\
	p_string_append,\
	p_string_character,\
	p_string_substring,\
	p_string_split\
}
#define d_string(s,c...) f_string_new(NULL,(s),##c)
extern const char v_string_kind[];
typedef struct o_string {
	d_object_head;
	struct {
		unsigned int constant:1;
	} s_flags;
	char *content;
	size_t size, length;
	void (*m_trim)(struct o_string *);
	void (*m_append)(struct o_string *, struct o_string *);
	char (*m_character)(struct o_string *, size_t);
	struct o_string *(*m_substring)(struct o_string *, size_t, size_t);
	struct o_array *(*m_split)(struct o_string *, char character);
	void (*m_truncate)(struct o_string *, size_t);
} o_string;
extern void p_string_hooking(struct o_string *object);
extern struct o_string *f_string_new(struct o_string *supplied, size_t size,
									 const char *format, ...);
extern struct o_string *f_string_new_args(struct o_string *supplied,
										  size_t size, const char *format,
										  va_list parameters);
extern struct o_string *f_string_new_constant(struct o_string *supplied,
											  const char *content);
extern char *p_string_format_object_kind(char *buffer, size_t size,
										 char *format, va_list parameters);
extern char *p_string_format_object_content(char *buffer, size_t size,
											char *format, va_list parameters);
extern void p_string_delete(struct o_object *object);
extern int p_string_compare(struct o_object *object, struct o_object *other);
extern t_hash_value p_string_hash(struct o_object *object);
extern char *p_string_string(struct o_object *object, char *data, size_t size);
extern struct o_object *p_string_clone(struct o_object *object);
extern void p_string_trim(struct o_string *object);
extern void p_string_append(struct o_string *object, struct o_string *other);
extern char p_string_character(struct o_string *object, size_t position);
extern struct o_string *p_string_substring(struct o_string *object,
										   size_t begin, size_t length);
extern struct o_array *p_string_split(struct o_string *object, char character);
extern void p_string_truncate(struct o_string *object, size_t length);
#endif
