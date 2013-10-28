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
#include <serenity/ground/ground.h>
d_exception_define(generic, 1, "generic exception");
d_exception_define(arithmetic, 2, "arithmetic exception");
d_exception_define(range, 3, "index out of range exception");
void check (int l, int r) {
	if (r < l)
		d_throw(v_exception_range, "wrong range specified");
}

void function2 (int l, int r) {
	struct s_exception *exception = NULL;
	d_try {
		check(l,r);
	} d_catch(exception) {
		d_exception_dump(stderr, exception);
		d_raise;
	}
	d_endtry;
}

void function1 (void) {
	struct s_exception *exception = NULL;
	d_try {
		function2(5, 2);
	} d_catch_kind(v_exception_range, exception) {
		d_exception_dump(stderr, exception);
		d_raise;
	} d_catch(exception) {
		/* you can handle this shit */
	}
	d_endtry;
}

int main (int argc, char *argv[]) {
	struct s_exception *exception = NULL;
	d_try {
		function1();
	} d_catch(exception) {
		d_exception_dump(stderr, exception);
		/* we can stop here */
	}
	d_endtry;
	return 0;
}
