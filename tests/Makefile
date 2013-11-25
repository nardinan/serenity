object = test$(number).o
cc = gcc
cflags = -Wall -I$(include_dir) `libusb-config --cflags` -Wno-variadic-macros -std=c99 -c -pedantic
lflags = -Wall
library_links = -L$(library_dir) -lserenity_ground -lserenity_structures -lserenity_crypto -lserenity_infn `libusb-config --libs`
exec = test$(number).bin

all: $(object)
	$(cc) $(lflags) $(object) -o $(exec) $(library_links)

test$(number).o: test$(number).c
	$(cc) $(cflags) test$(number).c 

clean:
	rm -f *.o
	rm -f *.bin

