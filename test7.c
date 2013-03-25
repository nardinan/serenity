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
#include "o_string.h"
#include "o_array.h"
#include "o_pool.h"
int main (int argc, char *argv[]) {
	struct s_exception *exc;
	struct o_pool *pool = f_pool_new(NULL);
	struct o_string string = d_string_constant("hello,salut,what,we,,have"),
					*singleton;
	struct o_array *array, *clone;
	int index;
	d_try {
		array = string.m_split(&string, ',');
		for (index = 0; index < array->size; index++) {
			singleton = (struct o_string *)array->m_obtain(array,index);
			if (singleton)
				printf("\t%s\n", singleton->content);
			else
				printf("\t<null>\n");
		}
		printf("in the array we have %zd elements and %zd spaces\n",
			   array->filled, array->size);
		clone = d_clone(array, struct o_array);
		d_release(array);
		for (index = 0; index < clone->size; index++) {
			singleton = (struct o_string *)clone->m_obtain(clone, index);
			if (singleton)
				printf("\t%s\n", singleton->content);
			else
				printf("\t<null>\n");
		}
		d_release(clone);
		printf("creating an array from a list\n");
		d_pool_begin(pool) {
			array = f_array_new_list(NULL, 4,
									 d_P(d_string_pure("This"),
										 struct o_string),
									 d_P(d_string_pure("Is"),
										 struct o_string),
									 d_P(d_string_pure("The"),
										 struct o_string),
									 d_P(d_string_pure("Time"),
										 struct o_string));
			for (index = 0; index < array->size; index++) {
				singleton = (struct o_string *)array->m_obtain(array, index);
				if (singleton)
					printf("\t%s\n", singleton->content);
				else
					printf("\t<null>\n");
			}
			d_release(array);
		} d_pool_end_flush;
		d_release(pool);
	} d_catch(exc) {
		d_exception_dump(stdout, exc);
	} d_endtry;
	f_memory_flush(e_log_level_ever);
	return 0;
}
