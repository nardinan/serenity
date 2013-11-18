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
#include "o_stream.h"
const char v_stream_kind[] = "stream";
void p_stream_hooking(struct o_stream *object) {
	object->head.s_delegate.m_delete = p_stream_delete;
	/* keep default compare function */
	/* keep default hash function */
	object->head.s_delegate.m_string = p_stream_string;
	object->head.s_delegate.m_clone = p_stream_clone;
	object->m_write = p_stream_write;
	object->m_write_string = p_stream_write_string;
	object->m_write_stream = p_stream_write_stream;
	object->m_read_raw = p_stream_read_raw;
	object->m_read_line = p_stream_read_line;
	object->m_read = p_stream_read;
	object->m_size = p_stream_size;
	object->m_seek = p_stream_seek;
	object->m_blocking = p_stream_blocking;
}

extern struct o_stream *f_stream_new(struct o_stream *supplied, struct o_string *name, int descriptor) {
	struct o_stream *result;
	if ((result = (struct o_stream *) f_object_new(v_stream_kind, sizeof(struct o_stream), (struct o_object *)supplied))) {
		result->descriptor = descriptor;
		result->s_flags.supplied = d_true;
		result->s_flags.opened = d_true;
		result->name = d_retain(name, struct o_string);
		result->flags = fcntl(result->descriptor, F_GETFL);
		p_stream_hooking(result);
	}
	return result;
}

extern struct o_stream *f_stream_new_file(struct o_stream *supplied, struct o_string *name, const char *action, int permissions) {
	struct o_stream *result;
	if ((result = (struct o_stream *) f_object_new(v_stream_kind, sizeof(struct o_stream), (struct o_object *)supplied))) {
		p_stream_hooking(result);
		result->name = d_retain(name, struct o_string);
		result->flags = -1;
		switch (action[0]) {
			case 'r':
				result->flags = d_read_flags;
				if (action[1] == 'w')
					result->flags = d_write_read_flags;
				break;
			case 'w':
				result->flags = d_truncate_flags;
				if (action[1] == 'a')
					result->flags = d_append_flags;
				break;
		}
		if (result->flags != -1) {
			if ((result->descriptor = open(result->name->content, result->flags, permissions)) > -1)
				result->s_flags.opened = d_true;
			else
				d_throw(v_exception_unreachable, "unable to open the file");
		} else
			d_throw(v_exception_malformed, "malformed action format");
	}
	return result;
}

struct o_stream *f_stream_new_raw(struct o_stream *supplied, struct o_string *name, const char *raw, size_t bytes) {
	struct o_stream *result;
	FILE *output;
	if ((result = (struct o_stream *) f_object_new(v_stream_kind, sizeof(struct o_stream), (struct o_object *)supplied))) {
		p_stream_hooking(result);
		result->name = d_retain(name, struct o_string);
		result->flags = d_write_read_flags;
		result->s_flags.temporary = d_true;
		if ((output = tmpfile())) {
			result->descriptor = fileno(output);
			result->s_flags.opened = d_true;
			write(result->descriptor, raw, bytes);
			p_stream_seek(result, 0, e_stream_seek_begin);
		} else
			d_throw(v_exception_unreachable, "unable to open the temporary file");
	}
	return result;
}

void p_stream_delete(struct o_object *object) {
	struct o_stream *local_object;
	if ((local_object = d_object_kind(object, stream))) {
		if ((!local_object->s_flags.supplied) && (local_object->s_flags.opened))
			close(local_object->descriptor);
		d_release(local_object->name);
	} else
		d_throw(v_exception_kind, "object is not an instance of o_stream");
}

char *p_stream_string(struct o_object *object, char *data, size_t size) {
	struct o_stream *local_object;
	char *pointer = data;
	ssize_t written;
	if ((local_object = d_object_kind(object, stream))) {
		written = snprintf(pointer, size, "<file \"%s\" (%s) flags: ", local_object->name->content, (local_object->s_flags.opened)?"open":"close");
		written = ((written>size)?size:written);
		pointer += written;
		if (written < size) {
			if (local_object->flags != -1) {
				if ((local_object->flags&O_RDONLY) == O_RDONLY) {
					*pointer = 'r';
					if ((++written) < size)
						pointer++;
				} else if ((local_object->flags&O_RDWR) == O_RDWR) {
					*pointer = 'r';
					if ((++written) < size) {
						pointer++;
						*pointer = 'w';
						if ((++written) < size)
							pointer++;
					}
				} else if ((local_object->flags&O_WRONLY) == O_WRONLY) {
					*pointer = 'w';
					if ((++written) < size) {
						pointer++;
						if ((local_object->flags&O_APPEND) == O_APPEND) {
							*pointer = 'a';
							if ((++written) < size)
								pointer++;
						}
					}
				}
			} else {
				*pointer = 'n';
				if ((++written) < size) {
					pointer++;
					*pointer = 'o';
					if ((++written) < size)
						pointer++;
				}
			}
		}
		if (written < size) {
			*pointer = '>';
			if ((++written) < size)
				pointer++;
		}
		*pointer = '\0';
	} else
		d_throw(v_exception_kind, "object is not an instance of o_stream");
	return pointer;
}

struct o_object *p_stream_clone(struct o_object *object) {
	struct o_stream *result = NULL, *local_object;
	if ((local_object = d_object_kind(object, stream))) {
		result = (struct o_stream *)p_object_clone(object);
		if (local_object->s_flags.opened)
			result->descriptor = dup(local_object->descriptor);
		result->descriptor = dup(local_object->descriptor);
	} else
		d_throw(v_exception_kind, "object is not an instance of o_array");
	return (o_object *)result;
}

ssize_t p_stream_write(struct o_stream *object, size_t size, void *source) {
	ssize_t written = 0;
	if (object->s_flags.opened) {
		if (((object->flags&O_RDWR) == O_RDWR) || ((object->flags&O_WRONLY) == O_WRONLY))
			written = write(object->descriptor, source, size);
		else
			d_throw(v_exception_unsupported, "can't write in a read-only stream");
	} else
		d_throw(v_exception_closed, "can't write in a closed stream");
	return written;
}

ssize_t p_stream_write_string(struct o_stream *object, struct o_string *string) {
	return p_stream_write(object, string->m_length(string), string->content);
}

ssize_t p_stream_write_stream(struct o_stream *object, struct o_stream *source) {
	struct o_string *output = NULL;
	ssize_t readed, total;
	total = source->m_size(source);
	if ((output = source->m_read(source, NULL, total))) {
		if ((readed = output->size) == total) {
			p_stream_write(object, output->size, output->content);
			d_release(output);
		} else {
			d_release(output);
			d_throw(v_exception_stream, "size of file and readed bytes from source stream are different");
		}
	} else
		d_throw(v_exception_stream, "can't read from the source stream");
	return total;
}

ssize_t p_stream_read_raw(struct o_stream *object, unsigned char *buffer, size_t size) {
	ssize_t readed = 0;
	if (object->s_flags.opened) {
		if (((object->flags&O_RDWR) == O_RDWR) || ((object->flags&O_RDONLY) == O_RDONLY))
			readed = read(object->descriptor, buffer, size);
		else
			d_throw(v_exception_unsupported, "can't read from a write-only stream");
	} else
		d_throw(v_exception_closed, "can't read from a closed stream");
	return readed;
}

struct o_string *p_stream_read_line(struct o_stream *object, struct o_string *supplied, size_t size) {
	struct o_string *result = supplied;
	char character;
	ssize_t readed = 0;
	if (object->s_flags.opened) {
		if (((object->flags&O_RDWR) == O_RDWR) || ((object->flags&O_RDONLY) == O_RDONLY)) {
			if (!result)
				result = f_string_new(NULL, size, NULL);
			while ((readed < size) && (read(object->descriptor, &character, 1)))
				if ((character != '\n') && (character != '\0'))
					result->content[readed++] = character;
				else
					break;
			result->content[readed] = '\0';
			if (!readed) {
				if (!supplied)
					d_release(result);
				result = NULL;
			}
		} else
			d_throw(v_exception_unsupported, "can't read from a write-only stream");
	} else
		d_throw(v_exception_closed, "can't read from a closed stream");
	return result;
}

struct o_string *p_stream_read(struct o_stream *object, struct o_string *supplied, size_t size) {
	struct s_exception *exception = NULL;
	struct o_string *result = supplied;
	ssize_t readed;
	d_try {
		if (!result)
			result = f_string_new(NULL, size, NULL);
		if ((readed = p_stream_read_raw(object, (unsigned char *)result->content, size)) == 0) {
			if (!supplied)
				d_release(result);
			result = NULL;
		}
	} d_catch(exception) {
		d_exception_dump(stderr, exception);
		d_raise;
	} d_endtry;
	return result;
}

ssize_t p_stream_size(struct o_stream *object) {
	ssize_t result = 0;
	if (object->s_flags.opened) {
		if (((object->flags&O_RDWR) == O_RDWR) || ((object->flags&O_RDONLY) == O_RDONLY)) {
			result = (ssize_t)object->m_seek(object, 0, e_stream_seek_end);
			object->m_seek(object, 0, e_stream_seek_begin);
		} else
			d_throw(v_exception_unsupported, "can't read from a write-only stream");
	} else
		d_throw(v_exception_closed, "can't read from a closed stream");
	return result;
}

off_t p_stream_seek(struct o_stream *object, off_t offset, enum e_stream_seek whence) {
	int local_whence = SEEK_SET;
	off_t result = 0;
	if (object->s_flags.opened) {
		switch (whence) {
			case e_stream_seek_begin:
				local_whence = SEEK_SET;
				break;
			case e_stream_seek_current:
				local_whence = SEEK_CUR;
				break;
			case e_stream_seek_end:
				local_whence = SEEK_END;
				break;
			default:
				d_throw(v_exception_malformed, "malformed whence format");
		}
		result = lseek(object->descriptor, offset, local_whence);
	} else
		d_throw(v_exception_closed, "can't reposition cursor of a closed stream");
	return result;
}

void p_stream_blocking(struct o_stream *object, int blocking) {
	int flags;
	if (object->s_flags.opened) {
		flags = fcntl(object->descriptor, F_GETFL);
		if (blocking)
			flags &= ~O_NONBLOCK;
		else
			flags |= O_NONBLOCK;
		if (object->flags != flags)
			if (fcntl(object->descriptor, F_SETFL, flags) != -1)
				object->flags = flags;
	} else
		d_throw(v_exception_closed, "can't change flags of a closed stream");
}

