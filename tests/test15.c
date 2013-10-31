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
#include <serenity/structures/structures.h>
#include <serenity/structures/infn/infn.h>
int incoming_trb(struct o_trb *trb, void *data) {
	printf("- incoming device!\n");
	return 0;
}

int main(int argc, char *argv[]) {
	struct o_pool *pool = f_pool_new(NULL);
	struct o_stream *out_stream;
	struct s_exception *exception;
	struct o_trbs *trbs;
	struct s_trb_event *event = (struct s_trb_event *) d_malloc(sizeof(struct s_trb_event));
	d_try {
		d_pool_begin(pool) {
			out_stream = d_stdout;
			trbs = f_trbs_new(NULL);
			trbs->m_async_search(trbs, incoming_trb, 1000000, NULL);
			while (d_true) {
				usleep(1000000);
				printf("Searching for new miniTRB ...\n");
			}
			d_release(trbs);
			d_release(out_stream);
		} d_pool_end_clean;
		d_release(pool);
	} d_catch(exception) {
		d_exception_dump(stdout, exception);
	} d_endtry;
	d_free(event);
	f_memory_flush(e_log_level_ever);
	return 0;
}

