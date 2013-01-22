OBJSGRNDLIB = types.o log.o string.o hash.o list.o exception.o exceptions.o
CC = gcc
CFLAGS = -Wall -c -pedantic
LFLAGS = -Wall
EXEC = serenity_ground.so

all: $(OBJSGRNDLIB) $(MAINGRNDLIB)
	$(CC) $(LFLAGS) $(OBJSGRNDLIB) $(MAINGRNDLIB) -o $(EXEC) -shared

types.o: types.c types.h
	$(CC) $(CFLAGS) types.c

log.o: log.c log.h
	$(CC) $(CFLAGS) log.c

string.o: string.c string.h types.h logs.h
	$(CC) $(CFLAGS) string.c

hash.o: hash.c hash.h string.h
	$(CC) $(CFLAGS) hash.c

list.o: list.c list.h string.h
	$(CC) $(CFLAGS) list.c

exception.o: exception.c exception.h
	$(CC) $(CFLAGS) exception.c

exceptions.o: exceptions.c exceptions.h exception.h
	$(CC) $(CFLAGS) exceptions.c

test4.o: test4.c exceptions.h logs.h
	$(CC) $(CFLAGS) test4.c

clean:
	rm -f *.o
	rm $(EXEC)
