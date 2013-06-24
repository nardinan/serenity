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
#include "memory.h"
struct s_pointer *v_memory_root = NULL;
void *p_memory_insert(void *pointer, const char *file,
					  const char *function, unsigned int line, int inside) {
	struct s_pointer *node;
	int founded = d_false;
	if (inside) {
		node = v_memory_root;
		while (node) {
			if (node->pointer == pointer) {
				founded = d_true;
				break;
			}
			node = node->next;
		}
	}
	if ((!founded) &&
			(node = (struct s_pointer *) malloc(sizeof(struct s_pointer)))) {
		memset(node, 0, sizeof(struct s_pointer));
		node->pointer = pointer;
		strncpy(node->file, d_clean_file(file), d_memory_coordinate_size);
		strncpy(node->function, function, d_memory_coordinate_size);
		node->line = line;
		if (v_memory_root)
			v_memory_root->back = node;
		node->next = v_memory_root;
		node->back = NULL;
		v_memory_root = node;
	}
	return pointer;
}

void p_memory_remove(void *pointer, const char *file,
					const char *function, unsigned int line) {
	int result = d_false;
	struct s_pointer *node = v_memory_root;
	while (node) {
		if (node->pointer == pointer) {
			if (node->next)
				node->next->back = node->back;
			if (node->back)
				node->back->next = node->next;
			else
				v_memory_root = node->next;
			free(node);
			result = d_true;
			break;
		}
		node = node->next;
	}
	if (result == d_false)
		d_log(e_log_level_ever, "double free on %s:%s() (%d)",
			  file, function, line);
}

void f_memory_flush(enum e_log_level level) {
	struct s_pointer *node;
	while ((node = v_memory_root)) {
		d_log(level, "pointer %p allocated on %s::%s() (%d) is still there",
			  node->pointer, node->file, node->function, node->line);
		d_free(node->pointer);
	}
}
