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
#include <stdio.h>
#include <stdlib.h>
#include <serenity/ground/ground.h>
int main (int argc, char *argv[]) {
  char *test = NULL;
  size_t size = 0;
  test = f_string_trim(f_string_append(&test, "whut", &size));
  test = f_string_append(&test, "nothing", &size);
  printf("'%s' (allocated size: %lu)\n", test, size);
  d_free(test);
  return 0;
}
