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
#include "../ground/ground.h"
typedef struct s_person {
	d_list_node_head; /* must be in the head */
	char name[12], surname[12];
} s_person;

struct s_list_node *f_person_allocate(const char *name, const char *surname) {
	struct s_person *result;
	if ((result = (struct s_person *) d_malloc(sizeof(struct s_person)))) {
		snprintf(result->name, 12, "%s", name);
		snprintf(result->surname, 12, "%s", surname);
	} else
		d_die(d_error_malloc);
	return (struct s_list_node *)result;
}
int main (int argc, char *argv[]) {
	struct s_list *list = NULL;
	struct s_person *current = NULL;
	struct s_list_node *element = f_person_allocate("silvio", "berlusconi"), *support;
	f_list_init(&list);
	f_list_append(list, f_person_allocate("mario", "monti"), e_list_insert_head);
	f_list_append(list, element, e_list_insert_head);
	f_list_insert(list, f_person_allocate("mario", "rossi"), element);
	printf("elements in the list: %zu\n", list->fill);
	d_foreach(list, current, struct s_person)
		printf("%s %s\n", current->name, current->surname);
	/* delete */
	support = f_list_delete(list, element);
	d_free(support);
	printf("elements in the list (after delete): %zu\n", list->fill);
	d_foreach(list, current, struct s_person) {
		printf("%s %s\n", current->name, current->surname);
	}
}
