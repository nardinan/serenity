objects = o_idea.o o_aes.o
cc = gcc
cflags = -Wall -fPIC -Wno-variadic-macros -std=c99 -c -pedantic
lflags = -Wall
library_links = -L../ -L../../ground -lserenity_structures -lserenity_ground
executable = libserenity_crypto.so

all: $(objects)
	$(cc) $(lflags) $(objects) -o $(executable) $(library_links) -shared
	cp $(executable) ../..
	
o_idea.o: o_idea.c o_idea.h o_aes.h
	$(cc) $(cflags) o_idea.c 

o_aes.o: o_aes.c o_aes.h ../o_string.h
	$(cc) $(cflags) o_aes.c

clean:
	rm -f *.o
	rm -f $(executable)
