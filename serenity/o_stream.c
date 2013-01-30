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
#include "o_stream.h"
const char v_stream_kind[] = "stream";
void p_stream_hooking(struct o_stream *object) {
	object->head.s_delegate.m_delete = p_stream_delete;
	object->head.s_delegate.m_compare = p_stream_compare;
	/* keep default hash function */
	object->head.s_delegate.m_string = p_stream_string;
	object->head.s_delegate.m_clone = p_stream_clone;
	object->m_write = p_stream_write;
	object->m_write_binary = p_stream_write_binary;
	object->m_write_string = p_stream_write_string;
	object->m_read = p_stream_read;
	object->m_seek = p_stream_seek;
	object->m_blocking = p_stream_blocking;
}

extern struct o_stream *f_stream_new(struct o_stream *supplied,
									 struct o_string *name, int descriptor) {
	struct o_stream *result;
	if ((result = (struct o_stream *)
		 f_object_new(v_stream_kind, sizeof(struct o_stream),
					  (struct o_object *)supplied))) {
		result->descriptor = descriptor;
		result->s_flags.supplied = d_true;
		result->s_flags.opened = d_true;
		result->name = d_retain(name, struct o_string);
		result->flags = fcntl(result->descriptor, F_GETFL);
		p_stream_hooking(result);
	}
	return result;
}

extern struct o_stream *f_stream_new_file(struct o_stream *supplied,
										  struct o_string *name,
										  struct o_string *action) {
	struct o_stream *result;
	if ((result = (struct o_stream *)
		 f_object_new(v_stream_kind, sizeof(struct o_stream),
					  (struct o_object *)supplied))) {
		result->name = d_retain(name, struct o_string);
		result->flags = -1;
		switch (action->m_character(action, 0)) {
			case 'r':
				if (action->m_character(action, 1) == 'w')
					result->flags = d_write_read_flags;
				else
					result->flags = d_read_flags;
				break;
			case 'w':
				if (action->m_character(action, 1) == 'a')
					result->flags = d_append_flags;
				else
					result->flags = d_truncate_flags;
				break;
		}
		result->s_flags.supplied = d_false;
		result->s_flags.opened = d_false;
		p_stream_hooking(result);
		if (result->flags != -1) {
			if ((result->descriptor = open(result->name->content,
										   result->flags)) > -1)
				result->s_flags.opened = d_true;
			else
				d_throw(v_exception_unreachable, "unable to open the file");
		} else
			d_throw(v_exception_malformed, "malformed action format");
	}
	return result;
}

void p_stream_delete(struct o_object *object) {
	struct o_stream *local_object = (struct o_stream *)object;
	if ((!local_object->s_flags.supplied) && (local_object->s_flags.opened))
		close(local_object->descriptor);
	d_release(local_object->name);
}

int p_stream_compare(struct o_object *object, struct o_object *other) {
	struct o_stream *local_object = (struct o_stream *)object,
					*local_other = (struct o_stream *)other;
	return (int)local_object->descriptor-local_other->descriptor;
}

char *p_stream_string(struct o_object *object, char *data, size_t size) {
	struct o_stream *local_object = (struct o_stream *)object;
	ssize_t written;
	written = snprintf(data, size, "<file \"%s\" (%s) flags: ",
					   local_object->name->content,
					   (local_object->s_flags.opened)?"open":"close");
	data += written;
	if (written < size) {
		if (local_object->flags != -1) {
			if (local_object->flags&O_RDONLY) {
				*data= 'r';
				if ((++written) < size)
					data++;
			} else if (local_object->flags&O_RDWR) {
				*data = 'r';
				if ((++written) < size) {
					data++;
					*data= 'w';
					if ((++written) < size)
						data++;
				}
			} else if (local_object->flags&O_WRONLY) {
				*data= 'w';
				if ((++written) < size) {
					data++;
					if (local_object->flags&O_APPEND) {
						*data= 'a';
						if ((++written) < size)
							data++;
					}
				}
			}
		} else {
			*data = 'n';
			if ((++written) < size) {
				data++;
				*data= 'o';
				if ((++written) < size)
					data++;
			}
		}
	}
	if (written < size) {
		*data = '>';
		if ((++written) < size) {
			data++;
			*data= '\0';
		}
	}
	return data;
}

struct o_object *p_stream_clone(struct o_object *object) {
	struct o_stream *result = (struct o_stream *)p_object_clone(object),
					*local_object = (struct o_stream *)object;
	if (local_object->s_flags.opened)
		result->descriptor = dup(local_object->descriptor);
	return (o_object *)result;
}

ssize_t p_stream_write(struct o_stream *object, size_t size,
					   struct o_string *string) {
	ssize_t written = 0;
	if (object->s_flags.opened) {
		if ((object->flags&O_RDWR) || (object->flags&O_WRONLY))
			written = write(object->descriptor, string->content, size);
		else
			d_throw(v_exception_unsupported,
					"can't write in a read-only stream");
	} else
		d_throw(v_exception_closed, "can't write in a closed stream");
	return written;
}

ssize_t p_stream_write_binary(struct o_stream *object,
							  struct o_string *string) {
	return p_stream_write(object, string->size, string);
}

ssize_t p_stream_write_string(struct o_stream *object,
							  struct o_string *string) {
	return p_stream_write(object, string->m_length(string), string);
}

struct o_string *p_stream_read(struct o_stream *object, size_t size) {
	struct o_string *result = NULL;
	ssize_t readed;
	if (object->s_flags.opened) {
		if ((object->flags&O_RDWR) || (object->flags&O_RDONLY)) {
			if ((result = f_string_new(NULL, size, NULL))) {
				readed = read(object->descriptor, result->content, size);
				if (readed < 0) {
					d_release(result);
					result = NULL;
				}
			}
		} else
			d_throw(v_exception_unsupported,
					"can't read from a write-only stream");
	} else
		d_throw(v_exception_closed, "can't read from a closed stream");
	return result;
}

off_t p_stream_seek(struct o_stream *object, off_t offset,
					enum e_stream_seek whence) {
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
		d_throw(v_exception_closed,
				"can't reposition cursor of a closed stream");
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
