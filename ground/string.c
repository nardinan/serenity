/*
     serenity
     Copyright (C) 2012 Andrea Nardinocchi (nardinocchi@psychogames.net)
     
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
char *f_string_append(char **string, char *element) {
    size_t total = (d_strlen(*string)+d_strlen(element)+1);
    char *result;
    if ((result = (char *) malloc(total))) {
        snprintf(result, total, "%s%s", ((*string)?*string:""), element);
        result[total] = '\0';
        if (*string)
            free(*string);
        *string = result;
    } else
		d_die(d_error_malloc);
    return *string;
}

char *f_string_trim(char *string) {
    size_t length = d_strlen(string);
    char *begin = string, *final = (string+length)-1;
    while ((d_space_character(*begin) || (d_space_character(*final)) || (d_final_character(*final))) && (final >= begin)) {
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

char *f_string_format(char *buffer, size_t size, char symbols[], t_string_formatter functions[], char *format, ...) {
	va_list parameters;
	char *target = buffer, *pointer = format, *next;
	size_t dimension, remaining = (size-1), lower;
	unsigned int index;
	va_start(parameters, format);
	while ((next = strchr(pointer, '%'))) {
		if ((dimension = (next-pointer)) > 0)
			if ((lower = (dimension>remaining)?remaining:dimension)) {
				remaining -= lower;
				memcpy(target, pointer, lower);
				target += lower;
			}
		if ((pointer = (next+1))) {
			index = 0;
			while (symbols[index] != '^') {
				if (symbols[index] == *pointer) {
					next = functions[index](target, remaining, pointer, parameters);
					remaining -= (next-target);
					target = next;
					break;
				} else
					index++;
			}
			pointer++;
		}
	}
	va_end(parameters);
	if ((dimension = strlen(pointer)) > 0)
		if ((lower = (dimension>remaining)?remaining:dimension)) {
			remaining -= lower;
			memcpy(target, pointer, lower);
			target += lower;
		}
	*target = '\0';
	return buffer;
}
