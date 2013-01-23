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
#include "o_string.h"
const char v_string_kind[] = "string";
void p_string_hooking(struct o_string *object) {
	object->head.s_delegate.m_delete = p_string_delete;
	object->head.s_delegate.m_compare = p_string_compare;
	object->head.s_delegate.m_hash = p_string_hash;
	object->head.s_delegate.m_string = p_string_string;
	object->head.s_delegate.m_clone = p_string_clone;
	object->m_trim = p_string_trim;
	object->m_append = p_string_append;
}

struct o_string *f_string_new(struct o_string *supplied, int constant, size_t size, const char *content) {
	struct o_string *result;
	if ((result = (struct o_string *)f_object_new(v_string_kind, sizeof(struct o_string), (struct o_object *)supplied))) {
		if ((result->s_flags.constant = constant))
			result->content = (char *)content;
		else if (content)
			f_string_append(&(result->content), (char *)content);
		result->size = size;
		p_string_hooking(result);
	} else
		d_die(d_error_malloc);
	return result;
}

struct o_string *f_string_new_format(struct o_string *supplied, int constant, size_t size, const char *format, ...) {
	char *symbols = "@^";
	t_string_formatter functions[] = {
		p_string_format_object_content,
		p_string_format_object_kind,
	};
	struct o_string *result;
	va_list parameters;
	if ((result = (struct o_string *)f_object_new(v_string_kind, sizeof(struct o_string), (struct o_object *)supplied))) {
		va_start(parameters, format);
		if ((result->s_flags.constant = constant))
			result->content = p_string_format((char *)format, size, symbols, functions, (char *)format, parameters);
		else if ((result->content = (char *) malloc(size)))
			result->content = p_string_format(result->content, size, symbols, functions, (char *)format, parameters);
		result->size = size;
		p_string_hooking(result);
	} else
		d_die(d_error_malloc);
	return result;
}

char *p_string_format_object_kind(char *buffer, size_t size, char *format, va_list parameters) {
	struct o_object *object;
	size_t written = 0;
	if ((object = va_arg(parameters, struct o_object *)))
		written = snprintf(buffer, size, "<%s>", object->kind);
	return (buffer+written);
}

char *p_string_format_object_content(char *buffer, size_t size, char *format, va_list parameters) {
	struct o_object *object;
	char *pointer = buffer;
	if ((object = va_arg(parameters, struct o_object *)))
		pointer = object->s_delegate.m_string(object, buffer, size);
	return pointer;
}

void p_string_delete(struct o_object *object) {
	struct o_string *object_ = (struct o_string *)object;
	if ((!object_->s_flags.constant) && (object_->content))
		free(object_->content);
}

int p_string_compare(struct o_object *object, struct o_object *other) {
	struct o_string *object_ = (struct o_string *)object, *other_ = (struct o_string *)other;
	int result;
	if ((result = ((int)object_->size-(int)other_->size)) == 0)
		if ((object_->content) && (other_->content))
			result = strcmp(object_->content, other_->content);
	return result;
}

t_hash_value p_string_hash(struct o_object *object) {
	struct o_string *object_ = (struct o_string *)object;
	char *pointer = object_->content;
	t_hash_value result = 5381;
	int character;
	/* djb2 hash function */
	while ((character = *pointer++))
		result = ((result << 5) + result) + character;
	return result;
}

char *p_string_string(struct o_object *object, char *data, size_t size) {
 	struct o_string *object_ = (struct o_string *)object;
	size_t written;
	if (object_->content)
		written = snprintf(data, size, "%s", object_->content);
	else
		written = snprintf(data, size, "<null>");
	return (data+written);
}

struct o_object *p_string_clone(struct o_object *object) {
	struct o_string *result = (struct o_string *)p_object_clone(object), *object_ = (struct o_string *)object;
	if (!object_->s_flags.constant) {
		result->content = NULL;
		f_string_append(&result->content, object_->content);
	}
	return (struct o_object *)result;
}

void p_string_trim(struct o_string *object) {
	if (object->content)
		f_string_trim(object->content);
}

void p_string_append(struct o_string *object, struct o_string *other) {
	if (!object->s_flags.constant)
		if (other->content)
			f_string_append(&(object->content), other->content);
}
