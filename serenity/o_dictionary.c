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
#include "o_dictionary.h"
const char v_dictionary_kind[] = "dictionary";
void p_dictionary_hooking(struct o_dictionary *object) {
	object->head.s_delegate.m_delete = p_dictionary_delete;
	object->head.s_delegate.m_compare = p_dictionary_compare;
	object->head.s_delegate.m_hash = p_dictionary_hash;
	object->head.s_delegate.m_string = p_dictionary_string;
	object->head.s_delegate.m_clone = p_dictionary_clone;
}

void p_dictionary_hooking(struct o_dictionary *object) {
	
}

void p_dictionary_delete(struct o_object *object) {
	
}

int p_dictionary_compare(struct o_object *object, struct o_object *other) {
	
}

t_hash_value p_dictionary_hash(struct o_object *object) {
	
}

char *p_dictionary_string(struct o_object *object, char *data, size_t size) {
	
}

struct o_object *p_dictionary_clone(struct o_object *object) {

}
