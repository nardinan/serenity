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
#ifndef serenity_o_stream_h
#define serenity_o_stream_h
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>
#include "o_string.h"
#include "o_pool.h"
#define d_append_flags (O_WRONLY|O_CREAT|O_APPEND)
#define d_truncate_flags (O_WRONLY|O_CREAT|O_TRUNC)
#define d_read_flags (O_RDONLY)
#define d_write_read_flags (O_RDWR|O_CREAT)
#define d_stdout \
	f_stream_new(NULL,d_P(d_string(9,"<stdout>"),struct o_string),\
				fileno(stdout))
#define d_stderr \
	f_stream_new(NULL,d_P(d_string(9,"<stderr>"),struct o_string),\
				fileno(stderr))
#define d_stdin \
	f_stream_new(NULL,d_P(d_string(8,"<stdin>"),struct o_string),\
				fileno(stdin))
extern const char v_stream_kind[];
enum e_stream_seek {
	e_stream_seek_begin,
	e_stream_seek_current,
	e_stream_seek_end
} e_stream_seek;
typedef struct o_stream {
	d_object_head;
	struct o_string *name;
	int descriptor, flags;
	struct {
		unsigned int supplied:1;
		unsigned int opened:1;
	} s_flags;
	ssize_t (*m_write)(struct o_stream *, size_t, struct o_string *);
	ssize_t (*m_write_binary)(struct o_stream *, struct o_string *);
	ssize_t (*m_write_string)(struct o_stream *, struct o_string *);
	struct o_string *(*m_read)(struct o_stream *, size_t);
	off_t (*m_seek)(struct o_stream *, off_t, enum e_stream_seek);
	void (*m_blocking)(struct o_stream *, int);
} o_stream;
extern void p_stream_hooking(struct o_stream *object);
extern struct o_stream *f_stream_new(struct o_stream *supplied,
									struct o_string *name, int descriptor);
extern struct o_stream *f_stream_new_file(struct o_stream *supplied,
										  struct o_string *name,
										  struct o_string *action);
extern void p_stream_delete(struct o_object *object);
extern int p_stream_compare(struct o_object *object, struct o_object *other);
extern char *p_stream_string(struct o_object *object, char *data, size_t size);
extern struct o_object *p_stream_clone(struct o_object *object);
extern ssize_t p_stream_write(struct o_stream *object, size_t size,
							  struct o_string *string);
extern ssize_t p_stream_write_binary(struct o_stream *object,
									 struct o_string *string);
extern ssize_t p_stream_write_string(struct o_stream *object,
									 struct o_string *string);
extern struct o_string *p_stream_read(struct o_stream *object,
									  size_t size);
extern off_t p_stream_seek(struct o_stream *object, off_t offset,
						   enum e_stream_seek whence);
extern void p_stream_blocking(struct o_stream *object, int blocking);
#endif
