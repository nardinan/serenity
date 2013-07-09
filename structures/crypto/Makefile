OBJSGRNDLIB = o_idea.o o_aes.o
CC = gcc
CFLAGS = -Wall -Wno-variadic-macros -std=c99 -c -pedantic
LFLAGS = -Wall -L../ -L../../ground -lserenity_structures -lserenity_ground
EXEC = libserenity_crypto.so

all: $(OBJSGRNDLIB) $(MAINGRNDLIB)
	$(CC) $(LFLAGS) $(OBJSGRNDLIB) $(MAINGRNDLIB) -o $(EXEC) -shared
	cp $(EXEC) ../../tests/
	
o_idea.o: o_idea.c o_idea.h o_aes.h
	$(CC) $(CFLAGS) o_idea.c 

o_aes.o: o_aes.c o_aes.h ../o_string.h
	$(CC) $(CFLAGS) o_aes.c

clean:
	rm -f *.o
	rm $(EXEC)
