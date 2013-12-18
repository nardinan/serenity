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
	object->m_length = p_string_length;
	object->m_character = p_string_character;
	object->m_substring = p_string_substring;
	object->m_split = p_string_split;
	object->m_truncate = p_string_truncate;
}

struct o_string *f_string_new(struct o_string *supplied, size_t size, const char *format, ...) {
	struct o_string *result;
	va_list parameters;
	va_start(parameters, format);
	result = f_string_new_args(supplied, size, format, parameters);
	va_end(parameters);
	return result;
}

struct o_string *f_string_new_args(struct o_string *supplied, size_t size, const char *format, va_list parameters) {
	char *symbols = "@^";
	t_string_formatter functions[] = {
		p_string_format_object_content,
		p_string_format_object_kind,
	};
	struct o_string *result;
	if ((result = (struct o_string *) f_object_new(v_string_kind, sizeof(struct o_string), (struct o_object *)supplied))) {
		p_string_hooking(result);
		if ((result->size = (size+1))) {
			result->s_flags.constant = d_false;
			if ((result->content = (char *) d_calloc(result->size, sizeof(char)))) {
				if (format)
					result->content = f_string_format_args(result->content, (result->size-1), symbols, functions, (char *)format,
							parameters);
			} else
				d_die(d_error_malloc);
		}
	}
	return result;
}

struct o_string *f_string_new_constant(struct o_string *supplied, const char *content) {
	struct o_string *result;
	if ((result = (struct o_string *) f_object_new(v_string_kind, sizeof(struct o_string), (struct o_object *)supplied))) {
		p_string_hooking(result);
		result->s_flags.constant = d_true;
		if (content) {
			result->content = (char *)content;
			result->size = (d_strlen(result->content)+1);
		}
	}
	return result;
}

char *p_string_format_object_kind(char *buffer, size_t size, char *format, va_list parameters) {
	struct o_object *object;
	size_t written = 0;
	if ((object = va_arg(parameters, struct o_object *)))
		written = snprintf(buffer, size, "<%s>", object->kind);
	return buffer+((written>size)?size:written);
}

char *p_string_format_object_content(char *buffer, size_t size, char *format, va_list parameters) {
	struct o_object *object;
	char *pointer = buffer;
	if ((object = va_arg(parameters, struct o_object *)))
		pointer = object->s_delegate.m_string(object, buffer, size);
	return pointer;
}

void p_string_delete(struct o_object *object) {
	struct o_string *local_object;
	if ((local_object = d_object_kind(object, string))) {
		if ((!local_object->s_flags.constant) && (local_object->content))
			d_free(local_object->content);
	} else
		d_throw(v_exception_kind, "object is not an instance of o_string");
}

int p_string_compare(struct o_object *object, struct o_object *other) {
	struct o_string *local_object, *local_other;
	int result = p_object_compare(object, other);
	if ((local_object = d_object_kind(object, string)) && (local_other = d_object_kind(other, string)))
		result = d_strcmp(local_object->content, local_other->content);
	return result;
}

t_hash_value p_string_hash(struct o_object *object) {
	struct o_string *local_object;
	char *pointer;
	t_hash_value result = 0;
	if ((local_object = d_object_kind(object, string))) {
		pointer = local_object->content;
		if (!object->s_flags.hashed) {
			object->hash = 5381;
			/* djb2 hash function */
			while (*pointer) {
				object->hash = ((object->hash<<5)+object->hash)+*pointer;
				pointer++;
			}
			object->s_flags.hashed = d_true;
		}
		result = object->hash;
	} else
		d_throw(v_exception_kind, "object is not an instance of o_string");
	return result;
}

char *p_string_string(struct o_object *object, char *data, size_t size) {
	struct o_string *local_object;
	size_t written = 0;
	if ((local_object = d_object_kind(object, string))) {
		if (local_object->content)
			written = snprintf(data, size, "%s", local_object->content);
		else
			written = snprintf(data, size, "<null>");
	} else
		d_throw(v_exception_kind, "object is not an instance of o_string");
	return data+((written>size)?size:written);
}

struct o_object *p_string_clone(struct o_object *object) {
	struct o_string *result = NULL, *local_object;
	if ((local_object = d_object_kind(object, string))) {
		result = (struct o_string *)p_object_clone(object);
		if (!local_object->s_flags.constant) {
			if ((result->content = (char *) d_calloc(1, local_object->size)))
				memcpy(result->content, local_object->content, local_object->size);
			else
				d_die(d_error_malloc);
		}
	} else
		d_throw(v_exception_kind, "object is not an instance of o_string");
	return (struct o_object *)result;
}

void p_string_trim(struct o_string *object) {
	if (!object->s_flags.constant) {
		if (object->content)
			f_string_trim(object->content);
	} else
		d_throw(v_exception_constant, "operation not permitted with a constant");
}

void p_string_append(struct o_string *object, struct o_string *other) {
	if (!object->s_flags.constant) {
		if (other->content)
			f_string_append(&object->content, other->content, &object->size);
	} else
		d_throw(v_exception_constant, "operation not permitted with a constant");
}

size_t p_string_length(struct o_string *object) {
	return d_strlen(object->content);
}

char p_string_character(struct o_string *object, size_t position) {
	char result = 0;
	if (object->content) {
		if (position < object->size)
			result = object->content[position];
		else
			d_throw(v_exception_bound, "pointer out of bound");
	}
	return result;
}

struct o_string *p_string_substring(struct o_string *object, size_t begin, size_t length) {
	struct o_string *result = NULL;
	if (object->content) {
		if ((begin <= object->size) && ((begin+length) <= object->size))
			result = f_string_new(NULL, length, object->content+begin);
		else
			d_throw(v_exception_bound, "pointer out of bound");
	}
	return result;
}

struct o_array *p_string_split(struct o_string *object, char character) {
	struct o_array *result;
	struct o_string *value;
	char *pointer = object->content, *next;
	size_t recurrence = 0, index = 0, length = d_strlen(object->content);
	while ((next = strchr(pointer, character))) {
		if ((next-pointer) > 0)
			recurrence++;
		pointer = next+1;
	}
	if ((result = f_array_new(NULL, recurrence+1))) {
		pointer = object->content;
		while ((next = strchr(pointer, character))) {
			if ((next-pointer) > 0)
				if ((value = p_string_substring(object, (pointer-object->content), next-pointer))) {
					result->m_insert(result, (struct o_object *)value, index++);
					d_release(value);
				}
			pointer = next+1;
		}
		if ((value = p_string_substring(object, (pointer-object->content), (length-(pointer-object->content)+1)))) {
			result->m_insert(result, (struct o_object *)value, index);
			d_release(value);
		}
	}
	return result;
}

void p_string_truncate(struct o_string *object, size_t length) {
	if (object->content) {
		if (length < object->size)
			memset(object->content+length, '\0', object->size-length);
		else
			d_throw(v_exception_bound, "pointer out of bound");
	}
}

