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
#ifndef serenity_o_aes_h
#define serenity_o_aes_h
#include "../o_string.h"
#define d_aes_expanded_key_size 256 /* sizeof(unsigned int)*(4*(15+1)) */
extern const char v_aes_kind[];
extern unsigned int v_aes_rcon[], v_aes_sbox[], v_aes_sbox_inv[], v_aes_mul_2[],
       v_aes_mul_3[], v_aes_mul_9[], v_aes_mul_11[],
       v_aes_mul_13[], v_aes_mul_14[];
enum e_aes_block {
	e_aes_block_128 = 0,
	e_aes_block_192 = 1,
	e_aes_block_256 = 2
} e_aes_block;
typedef struct o_aes {
	d_object_head;
	unsigned char expanded_key[d_aes_expanded_key_size];
	enum e_aes_block block;
	struct o_string *(*m_crypt)(struct o_aes *object, struct o_string *string,
			int local);
	struct o_string *(*m_decrypt)(struct o_aes *object, struct o_string *string,
			int local);
} o_aes;
extern void p_aes_hooking(struct o_aes *object);
extern unsigned int *p_aes_key_core(unsigned char *key, unsigned int index,
		unsigned int matrix);
extern unsigned int *p_aes_key_column(unsigned int *previous,
		unsigned char *key, unsigned int index,
		unsigned int const_s, unsigned int jump);
extern unsigned int *p_aes_key_special_column(unsigned int *previous,
		unsigned char *key,
		unsigned int index,
		unsigned int const_s,
		unsigned int jump);
extern struct o_aes *f_aes_new(struct o_aes *supplied, enum e_aes_block block,
		unsigned char *key, size_t size);
extern int p_aes_compare(struct o_object *object, struct o_object *other);
extern t_hash_value p_aes_hash(struct o_object *object);
extern char *p_aes_string(struct o_object *object, char *data, size_t size);
extern struct o_string *p_aes_padding(struct o_string *string, int local,
		size_t const_n);
extern unsigned int p_aes_mix_column(unsigned int box);
extern unsigned int p_aes_mix_column_inverse(unsigned int box);
extern struct o_string *p_aes_crypt(struct o_aes *object,
		struct o_string *string, int local);
extern struct o_string *p_aes_decrypt(struct o_aes *object,
		struct o_string *string, int local);
#endif
