/*
     Serenity - a basic library.
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
#ifndef serenity_log_h
#define serenity_log_h
#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#define d_log_time_size 64
#define d_log_description_size 32
#define d_log_level_default e_log_level_low
#define d_clean_file(s) ((strrchr(s,'/')?:s-1)+1)
#define d_log(l,f,c...) p_log_write(stdout,l,d_clean_file(__FILE__),__FUNCTION__,__LINE__,f,##c)
#define d_err(l,f,c...) p_log_write(stderr,l,d_clean_file(__FILE__),__FUNCTION__,__LINE__,f,##c)
#define d_die(f,c...)\
	do{\
		p_log_write(stderr,e_log_level_ever,d_clean_file(__FILE__),__FUNCTION__,__LINE__,f,##c);\
		exit(1);\
	}while(0);
enum e_log_level {
	e_log_level_ever = 0,
	e_log_level_low,
	e_log_level_medium,
	e_log_level_high
};
extern enum e_log_level v_log_level;
extern const char v_log_level_description[][d_log_description_size];
extern void p_log_write(FILE *stream, enum e_log_level level, const char *file, const char *function, unsigned int line, const char *format, ...);
#endif
