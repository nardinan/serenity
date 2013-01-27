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
int main (int argc, char *argv[]) {
	size_t index;
	struct o_string *string[5], *result;
	string[0] = d_string("hello world");
	printf("character: %c\n", string[0]->m_character(string[0], 3));
	string[0]->m_trim(string[0]);
	string[1] = f_string_new(NULL, 128, "  object kind: %^ (content %@)[here we append:]", string[0], string[0]);
	string[1]->m_trim(string[1]);
	string[2] = d_retain(string[1], struct o_string);
	string[3] = d_clone(string[2], struct o_string);
	string[1]->m_append(string[1], string[0]);
	string[4] = f_string_new(NULL, 256, "  object description: %s (%@)", "nothing", string[1]);
	string[4]->m_trim(string[4]);
	printf("string[0]: %s\nstring[1]: %s\nstring[2]: %s\nstring[3]: %s\nstring[4]: %s\n", string[0]->content, string[1]->content, string[2]->content, string[3]->content, string[4]->content);
	for (index = 0; index < 1024; index++) {
		result = string[4]->m_substring(string[4], index, 10);
		if (result)
			printf("%s\n", result->content);
		d_release(result);
	}
	d_release(string[0]);
	d_release(string[1]);
	d_release(string[2]);
	d_release(string[3]);
	d_release(string[4]);
}
