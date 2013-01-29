/*
     serenity
     Copyright (C) 2012 Andrea Nardinocchi (andrea@nardinan.it)
     
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
#include <stdio.h>
#include "hash.h"
int compare (void *a, void *b) {
	char *as = (char *)a, *bs = (char *)b;
	return strcmp(as, bs);
}

t_hash_value calculate (void *a) {
	char *as = (char *)a;
	size_t length = strlen(as), index;
	t_hash_value result = 0;
	for (index = 0; index < length; index++)
		result += (int)as[index];
	printf("calculated hash: '%s' => %lld\n", as, result);
	return result;
}

int main (int argc, char *argv[]) {
    struct s_hash_table *hash = NULL;
	char *key = NULL, *value = NULL, *src = NULL;
	size_t key_size = 0, key_value = 0;
	f_hash_init(&hash, compare, calculate);
	f_hash_insert(hash, f_string_append(&key, "key1", &key_size),
				  f_string_append(&value, "value1", &key_value), s_true, NULL);
	key = value = NULL;
	key_size = key_value = 0;
	f_hash_insert(hash, f_string_append(&key, "key2", &key_size),
				  f_string_append(&value, "value2", &key_value), s_true, NULL);
	key = value = NULL;
	key_size = key_value = 0;
	f_hash_insert(hash, f_string_append(&key, "key3", &key_size),
				  f_string_append(&value, "value3", &key_value), s_true, NULL);
	key = value = NULL;
	key_size = key_value = 0;
	f_hash_insert(hash, f_string_append(&key, "key4", &key_size),
				  f_string_append(&value, "value4", &key_value), s_true, NULL);
	key = value = NULL;
	key_size = key_value = 0;
	f_hash_insert(hash, f_string_append(&key, "key5", &key_size),
				  f_string_append(&value, "value5", &key_value), s_true, NULL);
	key = value = NULL;
	key_size = key_value = 0;
	f_hash_insert(hash, f_string_append(&key, "key6", &key_size),
				  f_string_append(&value, "value6", &key_value), s_true, NULL);
	key = value = NULL;
	key_size = key_value = 0;
	f_hash_insert(hash, f_string_append(&key, "key7", &key_size),
				  f_string_append(&value, "value7", &key_value), s_true, NULL);
	key = value = NULL;
	key_size = key_value = 0;
	printf("there are %lld elements in the hash table\n", hash->fill);
	printf("value is: %s\n", f_hash_get(hash, f_string_append(&src, "key7", &key_value)));
	return 0;
}