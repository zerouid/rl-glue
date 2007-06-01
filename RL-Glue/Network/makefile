CC = gcc
CFLAGS = -ansi -pedantic -Wall -I../

all: RL_network.o RL_netapi.o

RL_network.o: RL_network.c
	$(CC) -c $(CFLAGS) $<

RL_netapi.o: RL_netapi.c RL_netapi.h
	$(CC) -c $(CFLAGS) $< 

clean:
	rm -fr *.o
	rm -fr *~
	rm -fr *.*~
	rm -fr \#*
