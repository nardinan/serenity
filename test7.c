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
#include "o_array.h"
int main (int argc, char *argv[]) {
	struct o_string *string[5], *result;
	struct o_array *array;
	string[0] = d_string("hello world");
	string[1] = d_string("hello to everybody");
	string[2] = d_string("nothing to do here");
	string[3] = d_string("what have you done now");
	string[4] = d_string("aloah");
	array = f_array_new(NULL, 4);
	array->m_insert(array, (struct o_object *)string[0], 0);
	array->m_insert(array, (struct o_object *)string[1], 1);
	array->m_insert(array, (struct o_object *)string[2], 2);
	array->m_insert(array, (struct o_object *)string[3], 3);
	d_release(string[0]);
	d_release(string[1]);
	d_release(string[2]);
	d_release(string[3]);
	array->m_insert(array, (struct o_object *)string[4], 2);
	d_release(string[4]);
	printf("in the array we have %zd elements\n", array->size);
	result = f_string_new_format(NULL, d_false, 512, "content: %@\n", array);
	printf("%s", result->content);
	d_release(result);
	d_release(array);
}
