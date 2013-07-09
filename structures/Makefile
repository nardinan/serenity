OBJSGRNDLIB = o_array.o o_dictionary.o o_exceptions.o o_filesystem.o o_object.o o_pool.o o_stream.o o_string.o
CC = gcc
CFLAGS = -Wall -Wno-variadic-macros -std=c99 -c -pedantic
LFLAGS = -Wall -L../ground -lserenity_ground
EXEC = libserenity_structures.so

all: $(OBJSGRNDLIB) $(MAINGRNDLIB)
	$(CC) $(LFLAGS) $(OBJSGRNDLIB) $(MAINGRNDLIB) -o $(EXEC) -shared
	cp $(EXEC) ../tests/

o_array.o: o_array.c o_array.h o_object.h
	$(CC) $(CFLAGS) o_array.c

o_dictionary.o: o_dictionary.c o_dictionary.h o_object.h o_array.h
	$(CC) $(CFLAGS) o_dictionary.c

o_exceptions.o: o_exceptions.c o_exceptions.h ../ground/ground.h
	$(CC) $(CFLAGS) o_exceptions.c

o_filesystem.o: o_filesystem.c o_filesystem.h o_object.h o_stream.h o_dictionary.h
	$(CC) $(CFLAGS) o_filesystem.c

o_object.o: o_object.c o_object.h o_exceptions.h
	$(CC) $(CFLAGS) o_object.c

o_pool.o: o_pool.c o_pool.h o_object.h
	$(CC) $(CFLAGS) o_pool.c

o_stream.o: o_stream.c o_stream.h o_string.h o_pool.h
	$(CC) $(CFLAGS) o_stream.c

o_string.o: o_string.c o_string.h o_array.h
	$(CC) $(CFLAGS) o_string.c

clean:
	rm -f *.o
	rm $(EXEC)
