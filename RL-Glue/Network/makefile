CC = gcc
CFLAGS = -ansi -pedantic -Wall -I../

all: RL_network.o RLnet.o

RL_network.o: RL_network.c
	$(CC) -c $(CFLAGS) $<

RLnet.o: RLnet.c RLnet.h
	$(CC) -c $(CFLAGS) $<

clean:
	rm -fr *.o
	rm -fr *~
	rm -fr *.*~
	rm -fr \#*
