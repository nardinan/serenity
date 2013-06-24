/*
 * serenity
 * Copyright (C) 2013 Andrea Nardinocchi (andrea@nardinan.it)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef serenity_string_h
#define serenity_string_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "memory.h"
#define d_string_arguent_size 24
#define d_strlen(a) ((a)?strlen(a):0)
#define d_strcmp(a,b)\
	(((a)&&(b))?strcmp(a,b):(((!a)&&(b))?-1:(((a)&&(!b))?1:0)))
#define d_space_character(a) (((a)==' ')||((a)=='\t'))
#define d_final_character(a) (((a)=='\0')||((a)=='\n'))
typedef char *(* t_string_formatter)(char *, size_t, char *, va_list);
extern char *f_string_append(char **string, char *suffix, size_t *space);
extern char *f_string_trim(char *string);
extern char *f_string_format(char *buffer, size_t size, char *symbols,
							 t_string_formatter functions[], char *format, ...);
extern char *f_string_format_args(char *buffer, size_t size, char *symbols,
								  t_string_formatter functions[], char *format,
								  va_list parameters);
extern char *p_string_format_skip(char *buffer, char *symbols);
#endif
