objects = o_array.o o_dictionary.o o_exceptions.o o_filesystem.o o_object.o o_pool.o o_stream.o o_string.o
cc = gcc
cflags = -Wall -fPIC -Wno-variadic-macros -std=c99 -c -pedantic
lflags = -Wall
library_links = -L../ground -lserenity_ground -lpthread
executable = libserenity_structures.so

all: $(objects)
	$(cc) $(lflags) $(objects) -o $(executable) $(library_links) -shared
	cp $(executable) ..

o_array.o: o_array.c o_array.h o_object.h
	$(cc) $(cflags) o_array.c

o_dictionary.o: o_dictionary.c o_dictionary.h o_object.h o_array.h
	$(cc) $(cflags) o_dictionary.c

o_exceptions.o: o_exceptions.c o_exceptions.h ../ground/ground.h
	$(cc) $(cflags) o_exceptions.c

o_filesystem.o: o_filesystem.c o_filesystem.h o_object.h o_stream.h o_dictionary.h
	$(cc) $(cflags) o_filesystem.c

o_object.o: o_object.c o_object.h o_exceptions.h
	$(cc) $(cflags) o_object.c

o_pool.o: o_pool.c o_pool.h o_object.h
	$(cc) $(cflags) o_pool.c

o_stream.o: o_stream.c o_stream.h o_string.h o_pool.h
	$(cc) $(cflags) o_stream.c

o_string.o: o_string.c o_string.h o_array.h
	$(cc) $(cflags) o_string.c

clean:
	rm -f *.o
	rm -f $(executable)
