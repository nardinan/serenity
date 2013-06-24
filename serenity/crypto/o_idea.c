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
#include "o_idea.h"
const char v_idea_kind[] = "idea";
void p_idea_hooking(struct o_idea *object) {
	/* keep default delete function */
	object->head.s_delegate.m_compare = p_idea_compare;
	object->head.s_delegate.m_hash = p_idea_hash;
	object->head.s_delegate.m_string = p_idea_string;
	/* keep default clone function */
	object->m_crypt = p_idea_crypt;
	object->m_decrypt = p_idea_decrypt;
}

void p_idea_key_shift_left(unsigned char *key, size_t length, int bits) {
	char byte;
	int step, shift_bytes = (bits/8), shift_bits = (bits%8);
	size_t index;
	for (step = 0; step < shift_bytes; step++) {
		byte = key[0];
		for (index = 0; index < (length-1); index++)
			key[index] = key[index+1];
		key[index] = byte;
	}
	if (shift_bits > 0) {
		byte = (key[0]>>(8-shift_bits));
		for (index = 0; index < (length-1); index++)
			key[index] = ((key[index]<<shift_bits)|
						  (key[index+1]>>(8-shift_bits)));
		key[index] = (key[index]<<shift_bits)|byte;
	}
}

/* recursive, hooked by p_idea_inverse_multiplication */
void _p_idea_inverse_multiplication(int value, int module, int *result) {
	int support[2];
	if (value == 0) {
		result[0] = value;
		result[1] = 0;
		result[2] = 1;
	} else {
		_p_idea_inverse_multiplication((module%value), value, result);
		support[0] = result[1];
		support[1] = result[2];
		result[2] = result[1];
		result[1] = (support[1]-support[0]*(module/value));
	}
}

void p_idea_inverse_multiplication(unsigned char *value,
								   unsigned char *result) {
	int a = (int)d_idea_couple_bytes_to_uint(value), solution[3];
	_p_idea_inverse_multiplication(a, 65537, solution);
	solution[1] = (solution[1]+65537)%65537;
	result[1] = (solution[1]&0xff);
	result[0] = ((solution[1]>>8)&0xff);
}

void p_idea_negative(unsigned char *value, unsigned char *result) {
	int solution = -(int)d_idea_couple_bytes_to_uint(value);
	result[1] = (solution&0xff);
	result[0] = ((solution>>8)&0xff);
}

struct o_idea *f_idea_new(struct o_idea *supplied, unsigned char *key,
						  size_t size) {
	struct o_idea *result;
	unsigned char *padded_key;
	int index, jump, step;
	if ((result = (struct o_idea *)
		 f_object_new(v_idea_kind, sizeof(struct o_idea),
					  (struct o_object *)supplied))) {
		p_idea_hooking(result);
		if (key) {
			if ((padded_key = (unsigned char *)
				 d_malloc(d_idea_expanded_key_bytes))) {
				memset(padded_key, '\0', d_idea_expanded_key_bytes);
				memcpy(padded_key, key, d_min(size, d_idea_expanded_key_bytes));
				for (index = 0, jump = 0; index < 6; index++,
					 jump += d_idea_expanded_key_bytes) {
					memcpy(&(result->expanded_key[e_idea_encrypt][jump]),
						   padded_key, d_idea_expanded_key_bytes);
					p_idea_key_shift_left(padded_key,
										  d_idea_expanded_key_bytes, 25);
				}
				memcpy(&(result->expanded_key[e_idea_encrypt][jump]),
					   padded_key, (d_idea_expanded_key_bytes/2));
				index = d_idea_expanded_key_size;
				p_mul(&(result->expanded_key[e_idea_encrypt][6]),
					  &(result->expanded_key[e_idea_decrypt][index-2]));
				p_neg(&(result->expanded_key[e_idea_encrypt][4]),
					  &(result->expanded_key[e_idea_decrypt][index-4]));
				p_neg(&(result->expanded_key[e_idea_encrypt][2]),
					  &(result->expanded_key[e_idea_decrypt][index-6]));
				p_mul(&(result->expanded_key[e_idea_encrypt][0]),
					  &(result->expanded_key[e_idea_decrypt][index-8]));
				index -= 8;
				for (step = 7, jump = 8; step > 0; step--, index -= 12,
					 jump += 12) {
					p_cop(&(result->expanded_key[e_idea_decrypt][index-2]),
						  &(result->expanded_key[e_idea_encrypt][jump+2]));
					p_cop(&(result->expanded_key[e_idea_decrypt][index-4]),
						  &(result->expanded_key[e_idea_encrypt][jump]));
					p_mul(&(result->expanded_key[e_idea_encrypt][jump+10]),
						  &(result->expanded_key[e_idea_decrypt][index-6]));
					p_neg(&(result->expanded_key[e_idea_encrypt][jump+6]),
						  &(result->expanded_key[e_idea_decrypt][index-8]));
					p_neg(&(result->expanded_key[e_idea_encrypt][jump+8]),
						  &(result->expanded_key[e_idea_decrypt][index-10]));
					p_mul(&(result->expanded_key[e_idea_encrypt][jump+4]),
						  &(result->expanded_key[e_idea_decrypt][index-12]));
				}
				p_cop(&(result->expanded_key[e_idea_decrypt][index-2]),
					  &(result->expanded_key[e_idea_encrypt][jump+2]));
				p_cop(&(result->expanded_key[e_idea_decrypt][index-4]),
					  &(result->expanded_key[e_idea_encrypt][jump]));
				p_mul(&(result->expanded_key[e_idea_encrypt][jump+10]),
					  &(result->expanded_key[e_idea_decrypt][index-6]));
				p_neg(&(result->expanded_key[e_idea_encrypt][jump+8]),
					  &(result->expanded_key[e_idea_decrypt][index-8]));
				p_neg(&(result->expanded_key[e_idea_encrypt][jump+6]),
					  &(result->expanded_key[e_idea_decrypt][index-10]));
				p_mul(&(result->expanded_key[e_idea_encrypt][jump+4]),
					  &(result->expanded_key[e_idea_decrypt][index-12]));
				d_free(padded_key);
			} else
				d_die(d_error_malloc);
		} else
			d_throw(v_exception_null,
					"key is undefined or content is a zero-length element");
	}
	return result;
}

int p_idea_compare(struct o_object *object, struct o_object *other) {
	struct o_idea *local_object, *local_other;
	int result = p_object_compare(object, other);
	if ((local_object = d_object_kind(object, idea)) &&
		(local_other = d_object_kind(other, idea)))
		if ((result = memcmp(local_object->expanded_key[0],
							 local_other->expanded_key[0],
							 d_idea_expanded_key_size)) == 0)
			result = memcmp(local_object->expanded_key[1],
							local_other->expanded_key[1],
							d_idea_expanded_key_size);
	return result;
}

t_hash_value p_idea_hash(struct o_object *object) {
	struct o_idea *local_object;
	size_t index;
	t_hash_value result = 0;
	if ((local_object = d_object_kind(object, idea))) {
		if (!object->s_flags.hashed) {
			object->hash = 5381;
			/* djb2 hash function */
			for (index = 0; index < d_idea_expanded_key_size; index++)
				object->hash = ((object->hash<<5)+object->hash)+
				local_object->expanded_key[0][index]+
				local_object->expanded_key[1][index];
			object->s_flags.hashed = d_true;
		}
		result = object->hash;
	}  else
		d_throw(v_exception_kind, "object is not an instance of o_idea");
	return result;
}

char *p_idea_string(struct o_object *object, char *data, size_t size) {
	struct o_idea *local_object;
	size_t written, local_written, local_size;
	int index;
	if ((local_object = d_object_kind(object, idea))) {
		written = snprintf(data, size, "<idea keys: ");
		written = ((written>size)?size:written);
		local_size = size-written;
		data += written;
		for (index = 0; (index < d_idea_expanded_key_size) &&
			 ((written+1) < size); index++, written += local_written,
			 data += local_written, local_size -= local_written) {
			local_written =
			snprintf(data, local_size, "%02x",
					 local_object->expanded_key[e_idea_encrypt][index]);
			local_written = ((local_written>local_size)?local_size:
							 local_written);
		}
		if (written < size) {
			*data = ',';
			data++;
		}
		for (index = 0; (index < d_idea_expanded_key_size) &&
			 ((written+1) < size); index++, written += local_written,
			 data += local_written, local_size -= local_written) {
			local_written =
			snprintf(data, local_size, "%02x",
					 local_object->expanded_key[e_idea_decrypt][index]);
			local_written = ((local_written>local_size)?local_size:
							 local_written);
		}
		if (written < size) {
			*data = '>';
			data++;
		}
	} else
		d_throw(v_exception_kind, "object is not an instance of o_idea");
	return data;
}

void p_idea_multiplication(unsigned char *left, unsigned char *right,
						   unsigned char *result) {
	int solution, a, b;
	a = d_idea_couple_bytes_to_uint(left);
	b = d_idea_couple_bytes_to_uint(right);
	if (a == 0)
		a = 65536;
	if (b == 0)
		b = 65536;
	solution = (a*b);
	if (solution) {
		a = (solution>>16)&0xffff;
		b = (solution&0xffff);
		if (b >= a)
			solution = (b-a);
		else
			solution = ((b-a)+65537)&0xffff;
	}
	if (a == b)
		solution = 1;
	result[1] = (solution&0xff);
	result[0] = ((solution>>8)&0xff);
}

void p_idea_sum(unsigned char *left, unsigned char *right,
				unsigned char *result) {
	unsigned int solution = (d_idea_couple_bytes_to_uint(left)+
							 d_idea_couple_bytes_to_uint(right));
	result[1] = (solution&0xff);
	result[0] = ((solution>>8)&0xff);
}

struct o_string *p_idea_execute(struct o_string *string, int local,
								unsigned char *expanded_key) {
	size_t index;
	int copy, executions, key;
	struct o_string *result;
	unsigned char space[4][2], support[2][2];
	result = p_aes_padding(string, local, 8);
	for (index = 0; index < result->size; index+=8) {
		for (copy = 0; copy < 4; copy++) {
			space[copy][0] = result->content[index+(copy*2)];
			space[copy][1] = result->content[index+(copy*2)+1];
		}
		for (executions = 0, key = 0; executions < 8; executions++, key += 12) {
			p_idea_multiplication(space[0], &(expanded_key[key]), space[0]);
			p_idea_sum(space[1], &(expanded_key[key+2]), space[1]);
			p_idea_sum(space[2], &(expanded_key[key+4]), space[2]);
			p_idea_multiplication(space[3], &(expanded_key[key+6]), space[3]);
			d_idea_couple_bytes_copy(support[0], space[2]);
			d_idea_couple_bytes_xor(space[2], space[0]);
			p_idea_multiplication(space[2], &(expanded_key[key+8]), space[2]);
			d_idea_couple_bytes_copy(support[1], space[1]);
			d_idea_couple_bytes_xor(space[1], space[3]);
			p_idea_sum(space[1], space[2], space[1]);
			p_idea_multiplication(space[1], &(expanded_key[key+10]), space[1]);
			p_idea_sum(space[2], space[1], space[2]);
			d_idea_couple_bytes_xor(space[0], space[1]);
			d_idea_couple_bytes_xor(space[3], space[2]);
			d_idea_couple_bytes_xor(space[1], support[0]);
			d_idea_couple_bytes_xor(space[2], support[1]);
		}
		p_idea_multiplication(space[0], &(expanded_key[key]),
							  (unsigned char *)&(result->content[index]));
		p_idea_sum(space[2], &(expanded_key[key+2]),
				   (unsigned char *)&(result->content[index+2]));
		p_idea_sum(space[1], &(expanded_key[key+4]),
				   (unsigned char *)&(result->content[index+4]));
		p_idea_multiplication(space[3], &(expanded_key[key+6]),
							  (unsigned char *)&(result->content[index+6]));
	}
	return result;
}

struct o_string *p_idea_crypt(struct o_idea *object, struct o_string *string,
							  int local) {
	return p_idea_execute(string, local, object->expanded_key[e_idea_encrypt]);
}

struct o_string *p_idea_decrypt(struct o_idea *object, struct o_string *string,
								int local) {
	return p_idea_execute(string, local, object->expanded_key[e_idea_decrypt]);
}
