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
#ifndef serenity_o_dictionary_h
#define serenity_o_dictionary_h
#include "o_object.h"
extern const char v_dictionary_kind[];
typedef struct o_dictionary {
	d_object_head;
	/* append here your attributes */
	/* append here your methods */
} o_dictionary;
extern void p_dictionary_hooking(struct o_dictionary *object);
extern void p_dictionary_delete(struct o_object *object);
extern int p_dictionary_compare(struct o_object *object, struct o_object *other);
extern t_hash_value p_dictionary_hash(struct o_object *object);
extern char *p_dictionary_string(struct o_object *object, char *data, size_t size);
extern struct o_object *p_dictionary_clone(struct o_object *object);
#endif
