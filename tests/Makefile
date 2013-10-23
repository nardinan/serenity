NUMBER = 14
OBJSGRNDLIB = test$(NUMBER).o
CC = gcc
CFLAGS = -Wall -I/opt/local/include/libusb -Wno-variadic-macros -std=c99 -c -pedantic
LFLAGS = -Wall
LIBSLINK = -L../ground -L../structures -L../structures/crypto -L../structures/infn -lserenity_structures -lserenity_ground -lserenity_crypto -lserenity_infn `libusb-legacy-config --libs`
EXEC = test$(NUMBER).bin

all: $(OBJSGRNDLIB) $(MAINGRNDLIB)
	cd ../ground; make
	cd ../structures; make
	cd ../structures/crypto; make
	cd ../structures/infn; make
	$(CC) $(LFLAGS) $(OBJSGRNDLIB) $(MAINGRNDLIB) -o $(EXEC) $(LIBSLINK)

test$(NUMBER).o: test$(NUMBER).c ../ground/ground.h ../structures/structures.h ../structures/crypto/crypto.h ../structures/infn/infn.h
	$(CC) $(CFLAGS) test$(NUMBER).c 

clean:
	rm -f *.o
	rm -f *.bin

cleanall:
	cd ../ground; make clean
	cd ../structures; make clean
	cd ../structures/crypto; make clean
	cd ../structures/infn; make clean
	rm -f *.o
	rm -f *.bin

