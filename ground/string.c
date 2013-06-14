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
#include "string.h"
char *f_string_append(char **string, char *suffix, size_t *space) {
	size_t size_string = d_strlen(*string), size_element = d_strlen(suffix),
			total;
	if ((total = size_string+size_element+1) >= *space) {
		if ((*string = (char *) d_realloc(*string, total)))
			*space = total;
		else
			d_die(d_error_malloc);
	}
	memcpy((*string)+size_string, suffix, size_element+1);
    return *string;
}

char *f_string_trim(char *string) {
    size_t length = d_strlen(string);
    char *begin = string, *final = (string+length)-1;
    while ((d_space_character(*begin) || (d_space_character(*final)) ||
			(d_final_character(*final))) && (final >= begin)) {
        if (d_space_character(*begin))
            begin++;
        if ((d_space_character(*final)) || (d_final_character(*final))) {
            *final = '\0';
            final--;
        }
    }
    if (begin > string)
        memmove(string, begin, d_strlen(begin)+1);
    return string;
}

char *f_string_format(char *buffer, size_t size, char *symbols,
					  t_string_formatter functions[], char *format, ...) {
	va_list parameters;
	va_start(parameters, format);
	f_string_format_args(buffer, size, symbols, functions, format, parameters);
	va_end(parameters);
	return buffer;
}

char *f_string_format_args(char *buffer, size_t size, char *symbols,
						   t_string_formatter functions[], char *format,
						   va_list parameters) {
	char *target = buffer, *pointer = format, *next, *last, *tail,
			argument[d_string_arguent_size];
	size_t dimension, remaining = size, lower, written;
	while ((next = strchr(pointer, '%'))) {
		if ((dimension = (next-pointer)) > 0)
			if ((lower = (dimension>remaining)?remaining:dimension)) {
				remaining -= lower;
				memcpy(target, pointer, lower);
				target += lower;
			}
		if ((pointer = (next+1))) {
			if ((last = p_string_format_skip(pointer, symbols))) {
				memset(argument, '\0', d_string_arguent_size);
				memcpy(argument, next, (last-next));
				if ((tail = strchr(symbols, *(last-1))) == NULL) {
					written = vsnprintf(target, remaining, argument,
										parameters);
					written = ((written>remaining)?remaining:written);
					remaining -= written;
					target += written;
				} else {
					next = functions[(tail-symbols)](target, remaining,
													 argument, parameters);
					remaining -= (next-target);
					target = next;
				}
				pointer = last;
			}
		}
	}
	if ((dimension = strlen(pointer)) > 0)
		if ((lower = (dimension>remaining)?remaining:dimension)) {
			remaining -= lower;
			memcpy(target, pointer, lower);
			target += lower;
		}
	*target = '\0';
	return buffer;
}

char *p_string_format_skip(char *buffer, char *symbols) {
	int error = d_false;
	while (strchr("#0-+ '", *buffer))
		buffer++;
	while (isdigit(*buffer))
		buffer++;
	if (*buffer == '.') {
		buffer++;
		if (isdigit(*buffer))
			while (isdigit(*buffer))
				buffer++;
		else
			error = d_true;
	}
	if (!error) {
		while (strchr("hljztLq", *buffer))
			buffer++;
		if ((strchr("diouXxfFeEgGaAcsb%", *buffer)) ||
			(strchr(symbols, *buffer)))
			buffer++;
		else
			error = d_true;
	}
	return (error)?NULL:buffer;
}
