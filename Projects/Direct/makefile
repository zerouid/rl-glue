CC = gcc
RL-Glue = ../../RL-Glue

# We need to search the RL-Glue directory RL_interface.h 
CFLAGS = -I$(RL-Glue)/ -ansi -pedantic -Wall

all: Glue Agent Environment Experiment
	$(CC) -o RL_main Build/RL_interface.o Build/RL_direct.o Build/ZeroAgent.o Build/mines.o Build/experiment.o

Glue: RL_interface.o RL_direct.o 
Agent: ZeroAgent.o
Environment: mines.o
Experiment: experiment.o

RL_interface.o: $(RL-Glue)/RL_interface.c $(RL-Glue)/RL_interface.h
	$(CC) -c $(CFLAGS) $< -o Build/RL_interface.o

RL_direct.o: $(RL-Glue)/Direct/RL_direct.c
	$(CC) -c $(CFLAGS) $< -o Build/RL_direct.o

ZeroAgent.o: ./Agent/ZeroAgent.c ./Agent/ZeroAgent.h
	$(CC) -c $(CFLAGS) $< -o Build/ZeroAgent.o

mines.o: ./Environment/mines.c ./Environment/mines.h
	$(CC) -c $(CFLAGS) $< -o Build/mines.o

experiment.o: ./Experiment/experiment.c
	$(CC) -c $(CFLAGS) $< -o Build/experiment.o

clean:
	rm -fr Build/*.o
	rm -fr RL_main

tidy:
	rm -fr *.*~
	rm -fr *~
	rm -fr \#*
