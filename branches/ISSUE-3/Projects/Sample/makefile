CC = gcc
RL-Glue = ../../RL-Glue

# We need to search the RL-Glue directory RL_interface.h 
CFLAGS = -I$(RL-Glue)/ -ansi -pedantic -Wall

all: Glue Agent Environment Experiment
	$(CC) -o RL_main Build/RL_interface.o Build/RL_direct.o Build/RL_direct_agent.o Build/RL_direct_environment.o Build/RL_direct_experiment.o Build/ZeroAgent.o Build/mines.o Build/main.o

Glue: RL_interface.o RL_direct.o RL_direct_agent.o RL_direct_environment.o RL_direct_experiment.o
Agent: ZeroAgent.o
Environment: mines.o
Experiment: main.o

RL_interface.o: $(RL-Glue)/RL_interface.c $(RL-Glue)/RL_interface.h
	$(CC) -c $(CFLAGS) $< -o Build/RL_interface.o

RL_direct.o: $(RL-Glue)/Direct/RL_direct.c
	$(CC) -c $(CFLAGS) $< -o Build/RL_direct.o

RL_direct_agent.o: $(RL-Glue)/Direct/Agent/RL_direct_agent.c
	$(CC) -c $(CFLAGS) $< -o Build/RL_direct_agent.o

RL_direct_environment.o: $(RL-Glue)/Direct/Environment/RL_direct_environment.c
	$(CC) -c $(CFLAGS) $< -o Build/RL_direct_environment.o

RL_direct_experiment.o: $(RL-Glue)/Direct/Experiment/RL_direct_experiment.c
	$(CC) -c $(CFLAGS) $< -o Build/RL_direct_experiment.o

ZeroAgent.o: ./Agent/ZeroAgent.c ./Agent/ZeroAgent.h
	$(CC) -c $(CFLAGS) $< -o Build/ZeroAgent.o

mines.o: ./Environment/mines.c ./Environment/mines.h
	$(CC) -c $(CFLAGS) $< -o Build/mines.o

main.o: ./Experiment/main.c
	$(CC) -c $(CFLAGS) $< -o Build/main.o

clean:
	rm -fr Build/*.o
	rm -fr RL_main

tidy:
	rm -fr *.*~
	rm -fr *~
	rm -fr \#*
