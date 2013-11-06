OBJSGRNDLIB = o_trb_event.o o_trb.o o_trbs.o
CC = gcc
CFLAGS = -Wall -fPIC -I/opt/local/include/libusb -Wno-variadic-macros -std=c99 -c -pedantic
LFLAGS = -Wall
LINKSLIB = -L../ -L../../ground -lm -lserenity_structures -lserenity_ground `libusb-legacy-config --libs` -lpthread
EXEC = libserenity_infn.so

all: $(OBJSGRNDLIB) $(MAINGRNDLIB)
	$(CC) $(LFLAGS) $(OBJSGRNDLIB) $(MAINGRNDLIB) -o $(EXEC) $(LINKSLIB) -shared
	cp $(EXEC) ../..
	
o_trb_event.o: o_trb_event.c o_trb_event.h ../o_object.h
	$(CC) $(CFLAGS) o_trb_event.c

o_trb.o: o_trb.c o_trb.h o_trb_event.h ../o_string.h ../o_stream.h
	$(CC) $(CFLAGS) o_trb.c 

o_trbs.o: o_trbs.c o_trbs.h o_trb.h
	$(CC) $(CFLAGS) o_trbs.c
clean:
	rm -f *.o
	rm -f $(EXEC)
