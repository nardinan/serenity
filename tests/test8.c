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
#include <serenity/structures/structures.h>
int main (int argc, char *argv[]) {
	struct o_pool *pool = f_pool_new(NULL);
	struct o_stream *out, *in;
	struct o_string *readed = NULL;
	d_pool_begin(pool) {
		out = d_stdout;
		in = d_stdin;
		out->m_write_string(out, d_P(d_string(128, "hello! from stream %@.. and what's your name? ", out), struct o_string));
		readed = in->m_read(in, 64);
		if (readed) {
			if (readed->m_character(readed, readed->m_length(readed)-1) == '\n')
				readed->m_truncate(readed, (readed->m_length(readed)-1));
			out->m_write_string(out, d_P(d_string(128, "hi %@!\n", readed), struct o_string));
			d_release(readed);
		}
	} d_pool_end_flush;
	pool->m_clean(pool, d_true); /* d_true remove every pointer from the pool */
	d_release(pool);
	d_release(out);
	d_release(in);
	f_memory_flush(e_log_level_ever);
	return 0;
}
