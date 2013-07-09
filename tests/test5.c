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
#include "../ground/ground.h"
char *f_formatter_length(char *buffer, size_t size, char *template,
		va_list parameters) {
	char *value = NULL;
	size_t written = 0;
	value = va_arg(parameters, char*);
	written = snprintf(buffer, size, "%lu", d_strlen(value));
	return buffer+((written>size)?size:written);
}

int main (int argc, char *argv[]) {
	char target[256], *symbols = "@";
	t_string_formatter functions[] = {f_formatter_length};
	f_string_format(target, 256, symbols, functions, "%d is a number, "
			"'%s' is a phrase and '%c' is a character and length is %@",
			12, "hello, how are you?", 'p', "calculate strlen here");
	printf("'%s'\n", target);
}
