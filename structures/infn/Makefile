objects = o_trb_event.o o_trb.o o_trbs.o
cc = gcc -g
default_cflags = -Wall -fPIC `libusb-config --cflags` -Wno-variadic-macros -std=c99 -c -pedantic
ifdef SUPPORT_TRB_0x1313
	cflags = $(default_cflags) -Dd_version_0x1313="enabled"
else
	cflags = $(default_cflags)
endif
lflags = -Wall
library_links = -L../ -L../../ground -lm -lserenity_structures -lserenity_ground `libusb-config --libs` -lpthread
executable = libserenity_infn.so

all: $(objects)
	$(cc) $(lflags) $(objects) -o $(executable) $(library_links) -shared
	cp $(executable) ../..
	
o_trb_event.o: o_trb_event.c o_trb_event.h ../o_object.h
	$(cc) $(cflags) o_trb_event.c

o_trb.o: o_trb.c o_trb.h o_trb_event.h ../o_string.h ../o_stream.h
	$(cc) $(cflags) o_trb.c 

o_trbs.o: o_trbs.c o_trbs.h o_trb.h
	$(cc) $(cflags) o_trbs.c
clean:
	rm -f *.o
	rm -f $(executable)
