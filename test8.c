/*
     serenity
     Copyright (C) 2013 Andrea Nardinocchi (nardinocchi@psychogames.net)
     
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
#include "o_pool.h"
#include "o_stream.h"
int main (int argc, char *argv[]) {
	struct o_pool *pool = f_pool_new(NULL);
	struct o_stream *out = d_stdout(pool), *in = d_stdin(pool);
	struct o_string *readed = NULL;
	out->m_write_all(out, P(pool, d_string(128, "hello! from stream %@ what's your name?", out), struct o_string));
	readed = in->m_read(in, 64);
	if (readed) {
		if (readed->m_character(readed, readed->length-1) == '\n')
			readed->m_truncate(readed, (readed->length-1));
		out->m_write_all(out, P(pool, d_string(128, "hi %@!\n", readed), struct o_string));
		d_release(readed);
	}
	pool->m_clean(pool, d_true); /* with d_true remove _every_ pointer in the pool */
	d_release(pool);
	return 0;
}
