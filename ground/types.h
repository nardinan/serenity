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
#ifndef serenity_types_h
#define serenity_types_h
#define d_true 1
#define d_false 0
typedef struct s_bool {
	unsigned int flag:1;
} s_bool;
extern const struct s_bool s_true, s_false;
#define d_is_equal(b,o) ((b).flag==(o).flag)
#define d_is_true(b) ((b).flag)
#define d_min(a,b) (((a)>(b))?(b):(a))
#define d_max(a,b) (((a)<(b))?(b):(a))
#endif

