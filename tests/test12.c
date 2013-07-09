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
#include "../structures/structures.h"
int main(int argc, char *argv[]) {
	struct o_pool *pool = f_pool_new(NULL);
	struct o_stream *out_stream;
	struct o_dictionary *dictionary, *clone;
	d_pool_begin(pool) {
		out_stream = d_stdout;
		dictionary = f_dictionary_new(NULL);
		dictionary->m_insert(dictionary,
				d_P(d_string_pure("key 1"),
					struct o_object),
				d_P(d_string_pure("I have no idea"),
					struct o_object));
		dictionary->m_insert(dictionary,
				d_P(d_string_pure("Key 2"),
					struct o_object),
				d_P(d_string_pure("What I am doing"),
					struct o_object));
		dictionary->m_insert(dictionary,
				d_P(d_string_pure("Key 3"),
					struct o_object),
				d_P(d_string_pure("Here"),
					struct o_object));
		dictionary->m_insert(dictionary,
				d_P(d_string_pure("Key 4"),
					struct o_object),
				d_P(d_string_pure("Forever a stone"),
					struct o_object));
		out_stream->m_write_string(out_stream,
				d_P(d_string(512, "%@\n", dictionary),
					struct o_string));
		clone = d_clone(dictionary, struct o_dictionary);
		d_release(dictionary);
		out_stream->m_write_string(out_stream,
				d_P(d_string(512, "(clone) %@\n", clone),
					struct o_string));
		d_release(clone);
		d_release(out_stream);
	} d_pool_end_flush;
	d_release(pool);
	f_memory_flush(e_log_level_ever);
	return 0;
}
