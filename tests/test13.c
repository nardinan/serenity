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
int main(int argc, char *argv[]) {
  struct o_pool *pool = f_pool_new(NULL);
  struct o_stream *out_stream;
  struct o_filesystem *filesystem;
  struct o_stream *file[3] = {NULL};
  struct s_exception *exception;
  int index;
  s_bool wrong = s_false;
  d_try {
    d_pool_begin(pool) {
      out_stream = d_stdout;
      if (argc > 4) {
        if (strcmp(argv[1], "C") == 0) {
          filesystem = f_filesystem_new(NULL);
          for (index = 3; index < argc; index += 2)
            if ((file[0] = f_stream_new_file(NULL, d_SP(argv[index]), "r", 0777))) {
              filesystem->m_insert(filesystem, d_SP(argv[index+1]), file[0]);
              d_release(file[0]);
            }
          file[1] = f_stream_new_file(NULL, d_SP(argv[2]), "w", 0755);
          filesystem->m_store(filesystem, file[1]);
          d_release(file[1]);
          d_release(filesystem);
        } else if (strcmp(argv[1], "D") == 0) {
          if ((file[0] = f_stream_new_file(NULL, d_SP(argv[2]), "r", 0777))) {
            if ((filesystem = f_filesystem_new_stream(NULL, file[0]))) {
              if ((file[1] =filesystem->m_get(filesystem, d_SP(argv[4])))) {
                file[2] = f_stream_new_file(NULL, d_SP(argv[3]), "w", 0755);
                file[2]->m_write_stream(file[2], file[1]);
                d_release(file[2]);
              }
              d_release(filesystem);
            }
            d_release(file[0]);
          }
        } else
          wrong = s_true;
      } else
        wrong = s_true;
      if (d_is_true(wrong)) {
        d_printf(out_stream, d_SP("Collapse multiple files:\n"));
        d_printf(out_stream, d_S(512, "\t%s C <destination.pack> <path#1> <label#1> <path#2> <label#2> ... \n",argv[0]));
        d_printf(out_stream, d_SP("Extract a file using label:\n"));
        d_printf(out_stream, d_S(512, "\t%s D <source.pack> <destination path> <label>\n",argv[0]));
      }
      d_release(out_stream);
    } d_pool_end_clean;
    d_release(pool);
  } d_catch(exception) {
    d_exception_dump(stdout, exception);
  } d_endtry;
  f_memory_flush(e_log_level_ever);
  return 0;
}

