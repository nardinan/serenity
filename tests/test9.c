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
#include "../structures/structures.h"
#include "../structures/crypto/crypto.h"
int main (int argc, char *argv[]) {
	unsigned char key[4] = "ABCD";
	struct o_pool *pool = f_pool_new(NULL);
	struct o_aes *aes = f_aes_new(NULL, e_aes_block_128, key, 4);
	struct o_string *input, *output;
	struct o_stream *out;
	d_pool_begin(pool) {
		out = d_stdout;
		out->m_write_string(out, d_P(d_string(512, "%@\n", aes),
					struct o_string));
		input = d_string_pure("Lorem ipsum dolor sit amet, consectetur "
				"adipisici elit, sed eiusmod tempor incidunt ut "
				"labore et dolore magna aliqua. Ut enim ad minim "
				"veniam, quis nostrud\n");
		out->m_write_string(out, d_P(d_string(512, "size of input before "
						"m_crypt: %zd\n", input->size),
					struct o_string));
		output = aes->m_crypt(aes, input, d_true);
		out->m_write_string(out, d_P(d_string(512, "size of input after "
						"m_crypt (padded): %zd\n",
						input->size),
					struct o_string));
		output = aes->m_decrypt(aes, input, d_true);
		out->m_write_string(out, input);
		pool->m_clean(pool, d_true);
		d_release(out);
		d_release(input);
	} d_pool_end_flush;
	d_release(pool);
	d_release(aes);
	f_memory_flush(e_log_level_ever);
	return 0;
}
