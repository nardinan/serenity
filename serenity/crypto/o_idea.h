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
#ifndef serenity_o_idea_h
#define serenity_o_idea_h
#include "../o_string.h"
#include "o_aes.h"
#define d_idea_expanded_key_size 104 /* (52(subkeys)*2) */
#define d_idea_expanded_key_bytes 16
#define d_idea_couple_bytes_to_uint(a)\
	((((unsigned int)(a)[0])<<8)|((unsigned int)(a)[1]))
#define d_idea_couple_bytes_copy(a,b)\
	do{\
		(a)[0]=(b)[0];\
		(a)[1]=(b)[1];\
	}while(0)
#define d_idea_couple_bytes_xor(a,b)\
	do{\
		(a)[0]^=(b)[0];\
		(a)[1]^=(b)[1];\
	}while(0)
#define p_mul p_idea_inverse_multiplication
#define p_shi p_idea_key_shift_left
#define p_neg p_idea_negative
#define p_cop d_idea_couple_bytes_copy
enum e_idea_key {
	e_idea_encrypt=0,
	e_idea_decrypt=1
};
extern const char v_idea_kind[];
typedef struct o_idea {
	d_object_head;
	unsigned char expanded_key[2][d_idea_expanded_key_size];
	struct o_string *(*m_crypt)(struct o_idea *object, struct o_string *string,
								int local);
	struct o_string *(*m_decrypt)(struct o_idea *object,
								  struct o_string *string, int local);
} o_idea;
extern void p_idea_hooking(struct o_idea *object);
extern void p_idea_key_shift_left(unsigned char *key, size_t length, int bits);
extern void p_idea_inverse_multiplication(unsigned char *value,
										  unsigned char *result);
extern void p_idea_negative(unsigned char *value, unsigned char *result);
extern struct o_idea *f_idea_new(struct o_idea *supplied, unsigned char *key,
								 size_t size);
extern int p_idea_compare(struct o_object *object, struct o_object *other);
extern t_hash_value p_idea_hash(struct o_object *object);
extern char *p_idea_string(struct o_object *object, char *data, size_t size);
extern void p_idea_multiplication(unsigned char *left, unsigned char *right,
								  unsigned char *result);
extern void p_idea_sum(unsigned char *left, unsigned char *right,
					   unsigned char *result);
extern struct o_string *p_idea_execute(struct o_string *string, int local,
									   unsigned char *expanded_key);
extern struct o_string *p_idea_crypt(struct o_idea *object,
									 struct o_string *string, int local);
extern struct o_string *p_idea_decrypt(struct o_idea *object,
									   struct o_string *string, int local);
#endif
