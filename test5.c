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
#include "string.h"
char *f_formatter_digits(char *buffer, size_t size, char *template, va_list parameters) {
	int value;
	size_t written;
	value = va_arg(parameters, int);
	if (*template == 'c')
		written = snprintf(buffer, size, "%c", value);
	else
		written = snprintf(buffer, size, "%d", value);
	return (buffer+written);
}

char *f_formatter_string(char *buffer, size_t size, char *template, va_list parameters) {
	char *value;
	size_t written;
	value = va_arg(parameters, char*);
	written = snprintf(buffer, size, "%s", value);
	return (buffer+written);
}

int main (int argc, char *argv[]) {
	char target[256], symbols[] = {'d', 'c', 's', '^'};
	t_string_formatter functions[] = {f_formatter_digits, f_formatter_digits, f_formatter_string};
	f_string_format(target, 256, symbols, functions, "%d is a number, '%s' is a phrase and '%c' is a character", 12, "hello, I'm Stephano", 'p');
	printf("'%s'\n", target);
}