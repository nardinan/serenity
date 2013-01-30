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
#include "o_pool.h"
#include "o_string.h"
#include "o_stream.h"
#include "o_aes.h"
int main (int argc, char *argv[]) {
	unsigned char key[16];
	key[0] = 0x00;
	key[1] = 0x01;
	key[2] = 0x02;
	key[3] = 0x03;
	key[4] = 0x04;
	key[5] = 0x05;
	key[6] = 0x06;
	key[7] = 0x07;
	key[8] = 0x08;
	key[9] = 0x09;
	key[10] = 0x0A;
	key[11] = 0x0B;
	key[12] = 0x0C;
	key[13] = 0x0D;
	key[14] = 0x0E;
	key[15] = 0x0F;
	struct o_pool *pool = f_pool_new(NULL);
	struct o_aes *aes = f_aes_new(NULL, e_aes_block_128, key, 16);
	struct o_string *input, *output;
	struct o_stream *out;
	d_pool_begin(pool) {
		out = d_stdout;
		out->m_write_string(out, d_P(d_string(512, "%@\n", aes), struct o_string));
		input = d_string_pure("Lorem ipsum dolor sit amet, consectetur adipisici elit, sed eiusmod tempor incidunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud");
		printf("size of input before m_crypt: %zd\n", input->size);
		output = aes->m_crypt(aes, input, d_true);
		printf("size of input after m_crypt (padded): %zd\n", input->size);
		output = aes->m_decrypt(aes, input, d_true);
		out->m_write_string(out, input);
		pool->m_clean(pool, d_true);
		d_release(out);
		d_release(aes);
	} d_pool_end;
	d_release(pool);
}
