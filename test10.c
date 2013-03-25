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
int main (int argc, char *argv[]) {
	unsigned char key[16] = {0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 0, 8};
	struct o_pool *pool = f_pool_new(NULL);
	struct o_idea *idea = f_idea_new(NULL, key, 16);
	struct o_string *input, *output;
	struct o_stream *out;
	d_pool_begin(pool) {
		out = d_stdout;
		out->m_write_string(out, d_P(d_string(512, "%@\n", idea),
									 struct o_string));
		input = d_string_pure("Lorem ipsum dolor sit amet, consectetur "
							  "adipisici elit, sed eiusmod tempor incidunt ut "
							  "labore et dolore magna aliqua. Ut enim ad minim "
							  "veniam, quis nostrud");
		out->m_write_string(out, d_P(d_string(512, "size of input before "
											  "m_crypt: %zd\n", input->size),
									 struct o_string));
		output = idea->m_crypt(idea, input, d_true);
		out->m_write_string(out, d_P(d_string(512, "size of input after "
											  "m_crypt (padded): %zd\n",
											  input->size),
									 struct o_string));
		output = idea->m_decrypt(idea, input, d_true);
		out->m_write_string(out, d_P(d_string(12, "output: "),
									 struct o_string));
		out->m_write_string(out, input);
		pool->m_clean(pool, d_true);
		d_release(out);
		d_release(input);
	} d_pool_end_flush;
	d_release(pool);
	d_release(idea);
	f_memory_flush(e_log_level_ever);
	return 0;
}
