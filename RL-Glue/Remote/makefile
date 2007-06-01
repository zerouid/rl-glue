CC = gcc
CFLAGS = -ansi -pedantic -Wall -I../

all: RL_remote.o RLnet.o

RL_remote.o: RL_remote.c
	$(CC) -c $(CFLAGS) $<

RLnet.o: ./Network/RLnet.c ./Network/RLnet.h
	$(CC) -c $(CFLAGS) $< 

clean:
	rm -fr *.o
	rm -fr *~
	rm -fr *.*~
	rm -fr \#*
