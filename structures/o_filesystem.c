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
#include "o_filesystem.h"
const char v_filesystem_kind[] = "filesystem";
void p_filesystem_hooking(struct o_filesystem *object) {
	object->head.s_delegate.m_delete = p_filesystem_delete;
	/* keep default compare function */
	/* keep default hash function */
	object->head.s_delegate.m_string = p_filesystem_string;
	object->head.s_delegate.m_clone = p_filesystem_clone;
	object->m_insert = p_filesystem_insert;
	object->m_get = p_filesystem_get;
	object->m_store = p_filesystem_store;
}

struct o_filesystem *f_filesystem_new(struct o_filesystem *supplied) {
	struct o_filesystem *result;
	if ((result = (struct o_filesystem *)
				f_object_new(v_filesystem_kind, sizeof(struct o_filesystem),
					(struct o_object *)supplied))) {
		p_filesystem_hooking(result);
		result->files = f_dictionary_new(NULL);
	}
	return result;
}

struct o_filesystem *f_filesystem_new_stream(struct o_filesystem *supplied,
		struct o_stream *stream) {
	struct o_filesystem *result;
	struct o_stream *record;
	struct o_string *content, *label;
	struct s_exception *exception = NULL;
	struct s_filesystem_header *header;
	size_t readed = 0, dimension, length;
	if ((result = (struct o_filesystem *)
				f_object_new(v_filesystem_kind, sizeof(struct o_filesystem),
					(struct o_object *)supplied))) {
		p_filesystem_hooking(result);
		result->files = f_dictionary_new(NULL);
		d_try {
			dimension = stream->m_size(stream);
			if ((content = stream->m_read(stream, dimension))) {
				length = sizeof(struct s_filesystem_header);
				while (readed < dimension) {
					header = ((struct s_filesystem_header*)
							(content->content+readed));
					if (f_endian_check() == d_big_endian)
						d_swap(header->bytes);
					readed += length;
					label = d_string_pure(header->name);
					if ((record = f_stream_new_raw(NULL, label,
									(content->content+readed),
									header->bytes))) {
						p_filesystem_insert(result, label, record);
						d_release(record);
					}
					d_release(label);
					readed += header->bytes;
				}
				d_release(content);
			}
		} d_catch(exception) {
			d_exception_dump(stderr, exception);
			d_raise;
		} d_endtry;
	}
	return result;
}

void p_filesystem_delete(struct o_object *object) {
	struct o_filesystem *local_object;
	if ((local_object = d_object_kind(object, filesystem)))
		d_release(local_object->files);
	else
		d_throw(v_exception_kind, "object is not an instance of o_filesystem");
}

char *p_filesystem_string(struct o_object *object, char *data, size_t size) {
	return NULL;
}

struct o_object *p_filesystem_clone(struct o_object *object) {
	struct o_filesystem *result = NULL, *local_object;
	if ((local_object = d_object_kind(object, filesystem))) {
		result = (struct o_filesystem *)p_object_clone(object);
		result->files = d_clone(local_object->files, struct o_dictionary);
	} else
		d_throw(v_exception_kind, "object is not an instance of o_filesystem");
	return (o_object *)result;
}

int p_filesystem_insert(struct o_filesystem *object, struct o_string *name,
		struct o_stream *stream) {
	return object->files->m_insert(object->files, (struct o_object *)name,
			(struct o_object *)stream);
}

struct o_stream *p_filesystem_get(struct o_filesystem *object,
		struct o_string *name) {
	return (struct o_stream *)object->files->m_get(object->files,
			(struct o_object *)name);
}

void p_filesystem_store(struct o_filesystem *object,
		struct o_stream *destination) {
	struct o_array *keys = NULL;
	struct o_stream *stream;
	struct o_string *key;
	struct s_filesystem_header header;
	struct s_exception *exception = NULL;
	size_t index;
	if ((keys = object->files->m_keys(object->files))) {
		for (index = 0; index < keys->filled; index++) {
			d_try {
				key = (struct o_string *)keys->m_get(keys, index);
				if ((stream = (struct o_stream *)
							object->files->m_get(object->files,
								(struct o_object *)key))) {
					memset(&header, 0, sizeof(struct s_filesystem_header));
					strncpy(header.name, key->content, d_filesystem_name_size);
					header.bytes = stream->m_size(stream);
					if (f_endian_check() == d_big_endian)
						d_swap(header.bytes);
					destination->m_write(destination,
							sizeof(struct s_filesystem_header),
							&header);
					destination->m_write_stream(destination, stream);
				}
			} d_catch(exception) {
				d_exception_dump(stderr, exception);
				d_raise;
			} d_endtry;
		}
		d_release(keys);
	}
}
