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
#ifndef serenity_memory_h
#define serenity_memory_h
#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "logs.h"
#define d_memory_coordinate_size 24
#define d_memory_sentry 0xfacefeed
#define d_memory_debug
#ifndef d_memory_debug
#define d_malloc(s) malloc(s)
#define d_calloc(c,s) calloc((c),(s))
#define d_realloc(p,s) realloc((p),(s))
#define d_free(p) free(p)
#else
#define d_malloc(s)\
	p_memory_insert(malloc(s),__FILE__,__FUNCTION__,__LINE__,0)
#define d_calloc(c,s)\
	p_memory_insert(calloc((c),(s)),__FILE__,__FUNCTION__,__LINE__,0)
#define d_realloc(p,s)\
	p_memory_insert(realloc((p),(s)),__FILE__,__FUNCTION__,__LINE__,1)
#define d_free(p)\
	do{\
		p_memory_remove(p,d_clean_file(__FILE__),__FUNCTION__,__LINE__);\
		free(p);\
	}while(0)
#endif
typedef struct s_pointer {
	void *pointer;
	char file[d_memory_coordinate_size], function[d_memory_coordinate_size];
	unsigned int line;
	struct s_pointer *next, *back;
} s_pointer;
extern struct s_pointer *v_memory_root;
extern void *p_memory_insert(void *pointer, const char *file,
							 const char *function, unsigned int line,
							 int inside);
extern void p_memory_remove(void *pointer, const char *file,
							const char *function, unsigned int line);
extern void f_memory_flush(enum e_log_level level);
#endif
