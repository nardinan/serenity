OBJSGRNDLIB = o_trb.o
CC = gcc
CFLAGS = -Wall -fPIC -I/opt/local/include/libusb -Wno-variadic-macros -std=c99 -c -pedantic
LFLAGS = -Wall
LINKSLIB = -L../ -L../../ground -lserenity_structures -lserenity_ground `libusb-legacy-config --libs`
EXEC = libserenity_infn.so

all: $(OBJSGRNDLIB) $(MAINGRNDLIB)
	$(CC) $(LFLAGS) $(OBJSGRNDLIB) $(MAINGRNDLIB) -o $(EXEC) $(LINKSLIB) -shared
	cp $(EXEC) ../../tests/
	
o_trb.o: o_trb.c o_trb.h ../o_string.h
	$(CC) $(CFLAGS) o_trb.c 

clean:
	rm -f *.o
	rm -f $(EXEC)
