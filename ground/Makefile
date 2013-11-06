objects = endian.o exception.o hash.o list.o log.o memory.o string.local.o types.o
cc = gcc
cflags = -Wall -fPIC -Wno-variadic-macros -std=c99 -c -pedantic
lflags = -Wall
executable = libserenity_ground.so

all: $(objects)
	$(cc) $(lflags) $(objects) -o $(executable) -shared
	cp $(executable) ..

endian.o: endian.c endian.h types.h
	$(cc) $(cflags) endian.c

exception.o: exception.c exception.h
	$(cc) $(cflags) exception.c

hash.o: hash.c hash.h string.local.h
	$(cc) $(cflags) hash.c

list.o: list.c list.h string.local.h
	$(cc) $(cflags) list.c

log.o: log.c log.h
	$(cc) $(cflags) log.c

memory.o: memory.c memory.h types.h logs.h
	$(cc) $(cflags) memory.c

string.local.o: string.local.c string.local.h types.h logs.h
	$(cc) $(cflags) string.local.c

types.o: types.c types.h
	$(cc) $(cflags) types.c

clean:
	rm -f *.o
	rm -f $(executable)
