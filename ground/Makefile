OBJSGRNDLIB = endian.o exception.o hash.o list.o log.o memory.o string.local.o types.o
CC = gcc
CFLAGS = -Wall -fPIC -Wno-variadic-macros -std=c99 -c -pedantic
LFLAGS = -Wall
EXEC = libserenity_ground.so

all: $(OBJSGRNDLIB) $(MAINGRNDLIB)
	$(CC) $(LFLAGS) $(OBJSGRNDLIB) $(MAINGRNDLIB) -o $(EXEC) -shared
	cp $(EXEC) ../tests/

endian.o: endian.c endian.h types.h
	$(CC) $(CFLAGS) endian.c

exception.o: exception.c exception.h
	$(CC) $(CFLAGS) exception.c

hash.o: hash.c hash.h string.local.h
	$(CC) $(CFLAGS) hash.c

list.o: list.c list.h string.local.h
	$(CC) $(CFLAGS) list.c

log.o: log.c log.h
	$(CC) $(CFLAGS) log.c

memory.o: memory.c memory.h types.h logs.h
	$(CC) $(CFLAGS) memory.c

string.local.o: string.local.c string.local.h types.h logs.h
	$(CC) $(CFLAGS) string.local.c

types.o: types.c types.h
	$(CC) $(CFLAGS) types.c

clean:
	rm -f *.o
	rm -f $(EXEC)
