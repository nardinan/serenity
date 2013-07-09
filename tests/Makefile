NUMBER = 1
OBJSGRNDLIB = test$(NUMBER).o
CC = gcc
CFLAGS = -Wall -Wno-variadic-macros -std=c99 -c -pedantic
LFLAGS = -Wall -L../ground -L../structures -L../structures/crypto -lserenity_structures -lserenity_ground -lserenity_crypto
EXEC = test$(NUMBER).bin

all: $(OBJSGRNDLIB) $(MAINGRNDLIB)
	cd ../ground; make
	cd ../structures; make
	cd ../structures/crypto; make
	$(CC) $(LFLAGS) $(OBJSGRNDLIB) $(MAINGRNDLIB) -o $(EXEC)

test$(NUMBER).o: test$(NUMBER).c ../ground/ground.h ../structures/structures.h ../structures/crypto/crypto.h
	$(CC) $(CFLAGS) test$(NUMBER).c 

clean:
	rm -f *.o
	rm -f *.bin

cleanall:
	cd ../ground; make clean
	cd ../structures; make clean
	cd ../structures/crypto; make clean
	rm -f *.o
	rm -f *.bin

