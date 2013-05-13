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
#include "o_pool.h"
#include "o_string.h"
#include "o_stream.h"
#include "o_idea.h"
int main(int argc, char *argv[]) {
	struct o_pool *pool = f_pool_new(NULL);
	struct o_stream *in_file = NULL, *out_file = NULL, *out_stream;
	struct o_string *output = NULL;
	struct s_exception *exception = NULL;
	size_t readed;
	d_pool_begin(pool) {
		out_stream = d_stdout;
		if (argc == 3) {
			d_try {
				out_stream->m_write_string(out_stream, d_P(d_string(512,
																	"%s -> "
																	"%s\n",
																	argv[1],
																	argv[2]),
														   struct o_string));
				in_file = f_stream_new_file(NULL,
								   d_P(d_string_pure(argv[1]),
									   struct o_string), "r");
				out_stream->m_write_string(out_stream,
										   d_P(d_string(128, "%@\n", in_file),
											   struct o_string));
				out_file = f_stream_new_file(NULL,
											 d_P(d_string_pure(argv[2]),
												 struct o_string), "w");
				while ((output = in_file->m_read(in_file, 512))) {
					readed = output->size;
					if (readed > 0)
						out_file->m_write_binary(out_file, output);
					d_release(output);
					if (!readed)
						break;
				}
				d_release(out_file);
				d_release(in_file);
			} d_catch(exception) {
				d_exception_dump(stderr, exception);
			} d_endtry;
		} else {
			out_stream->m_write_string(out_stream, d_P(d_string(512, "%s "
																"<src> "
																"<dst>\n",
															argv[0]),
												   struct o_string));
		}
	} d_pool_end_flush;
	d_release(pool);
	return 0;
}