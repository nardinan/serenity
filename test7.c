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
	struct o_string *result;
	struct o_array *array, *another;
	size_t index;
	struct o_string string = d_string_constant("hello,salut,what,c,,we,have,here");
	array = string.m_split(&string, ',');
	for (index = 0; index < array->size; index++) {
		result = (struct o_string *)array->m_obtain(array, index);
		if (result)
			printf("\t%s\n", result->content);
		else
			printf("\t<null>\n");
	}
	printf("in the array we have %zd elements and %zd spaces\n", array->filled, array->size);
	another = d_clone(array, struct o_array);
	d_release(result);
	d_release(array);
	printf("checking for clone:\n");
	for (index = 0; index < another->size; index++) {
		result = (struct o_string *)another->m_obtain(another, index);
		if (result)
			printf("\t%s\n", result->content);
		else
			printf("\t<null>\n");
	}
	d_release(another);
}
