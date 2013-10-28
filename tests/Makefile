OBJSGRNDLIB = test$(NUMBER).o
CC = gcc
CFLAGS = -Wall -I$(INC_DIR) -I/opt/local/include/libusb -Wno-variadic-macros -std=c99 -c -pedantic
LFLAGS = -Wall
LIBSLINK = -L$(LIB_DIR) -lserenity_structures -lserenity_ground -lserenity_crypto -lserenity_infn `libusb-legacy-config --libs`
EXEC = test$(NUMBER).bin

all: $(OBJSGRNDLIB) $(MAINGRNDLIB)
	$(CC) $(LFLAGS) $(OBJSGRNDLIB) $(MAINGRNDLIB) -o $(EXEC) $(LIBSLINK)

test$(NUMBER).o: test$(NUMBER).c
	$(CC) $(CFLAGS) test$(NUMBER).c 

clean:
	rm -f *.o
	rm -f *.bin

