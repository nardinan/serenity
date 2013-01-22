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
#include "o_string.h"
const char v_string_kind[] = "string";
struct o_string *f_string_new(char *content, int constant, struct o_string *supplied) {
	struct o_string *result;
	if ((result = (struct o_string *)f_object_new(v_string_kind, sizeof(struct o_string), (struct o_object *)supplied))) {
		if ((result->s_flags.constant = constant))
			result->content = content;
		else if (content)
			f_string_append(&(result->content), content);
		supplied->size = (result->content)?strlen(result->content):0;
		/* initialize with methods */
		result->head.s_delegate.m_delete = p_string_delete;
		result->head.s_delegate.m_compare = p_string_compare;
		result->head.s_delegate.m_hash = p_string_hash;
		result->head.s_delegate.m_string = p_string_string;
	} else
		d_die(d_error_malloc);
	return result;
}

void p_string_delete(struct o_object *object) {
	struct o_string *object_ = (struct o_string *)object;
	if ((!object_->s_flags.constant) && (object_->content))
		free(object_->content);
}

int p_string_compare(struct o_object *object, struct o_object *other) {
	struct o_string *object_ = (struct o_string *)object, *other_ = (struct o_string *)other;
	int result;
	if ((result = ((int)object_->size-(int)other_->size)) == 0)
		if ((object_->content) && (other_->content))
			result = strcmp(object_->content, other_->content);
	return result;
}

t_hash_value p_string_hash(struct o_object *object) {
	struct o_string *object_ = (struct o_string *)object;
	char *pointer = object_->content;
	t_hash_value result = 5381;
	int character;
	/* djb2 hash function */
	while ((character = *pointer++))
		result = ((result << 5) + result) + character;
	return result;
}

char *p_string_string(struct o_object *object, char *data, int size) {
 	struct o_string *object_ = (struct o_string *)object;
	if (object_->content)
		strncpy(data, object_->content, size);
	return data;
}
