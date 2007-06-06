# Agent Source Files
AGENTC = ./Agent/ZeroAgent.c
AGENTH = ./Agent/ZeroAgent.h

# Environment Source Files
ENVIRONMENTC = ./Environment/mines.c
ENVIRONMENTH = ./Environment/mines.h

# Experiment Source Files
EXPERIMENTC = ./Experiment/experiment.c
EXPERIMENTH = 

# Path to RL-Glue to find RL_interface and related files.
RL-GLUE = ../../RL-Glue

# Compiler flags
CC = gcc
CFLAGS = -I$(RL-GLUE)/ -ansi -pedantic -Wall
LDFLAGS =

# Objects we need to link against for a direct compilation
DIRECT_OBJECTS = Build/RL_direct.o

# Objects we need to link against for a remote compilation
REMOTE_OBJECTS = Build/RL_network.o Build/RL_netapi.o Build/RL_remote_agent.o Build/RL_remote_environment.o Build/RL_remote_experiment.o

# Build everything. Just as an example.  We only link with the DIRECT_OBJECTS here.
all: agent environment experiment glue direct network
	$(CC) -o RL_main Build/agent.o Build/environment.o Build/experiment.o Build/RL_interface.o $(REMOTE_OBJECTS)
	$(CC) -o RL_agent Build/agent.o Build/RL_netapi.o Build/RLnet_agent.o

# Dependencies
agent: $(AGENTC) $(AGENTH)
	$(CC) -c $(CFLAGS) $(AGENTC) -o Build/agent.o

environment: $(ENVIRONMENTC) $(ENVIRONMENTH)
	$(CC) -c $(CFLAGS) $(ENVIRONMENTC) -o Build/environment.o

experiment: $(EXPERIMENTC) $(EXPERIMENTH)
	$(CC) -c $(CFLAGS) $(EXPERIMENTC) -o Build/experiment.o

glue: $(RL-GLUE)/RL_interface.c $(RL-GLUE)/RL_interface.h
	$(CC) -c $(CFLAGS) $(RL-GLUE)/RL_interface.c -o Build/RL_interface.o

direct: RL_direct 

RL_direct: $(RL-GLUE)/Direct/RL_direct.c
	$(CC) -c $(CFLAGS) $(RL-GLUE)/Direct/RL_direct.c -o Build/RL_direct.o

network: RL_network RL_netapi RL_remote_agent RL_remote_environment RL_remote_experiment RLnet_agent

RL_network: $(RL-GLUE)/Remote/RL_network.c
	$(CC) -c $(CFLAGS) $(RL-GLUE)/Remote/RL_network.c -o Build/RL_network.o

RL_netapi: $(RL-GLUE)/Remote/RL_netapi.c
	$(CC) -c $(CFLAGS) $(RL-GLUE)/Remote/RL_netapi.c -o Build/RL_netapi.o

RL_remote_agent: $(RL-GLUE)/Remote/Agent/RL_remote_agent.c
	$(CC) -c $(CFLAGS) $(RL-GLUE)/Remote/Agent/RL_remote_agent.c -o Build/RL_remote_agent.o

RL_remote_environment: $(RL-GLUE)/Remote/Environment/RL_remote_environment.c
	$(CC) -c $(CFLAGS) $(RL-GLUE)/Remote/Environment/RL_remote_environment.c -o Build/RL_remote_environment.o

RL_remote_experiment: $(RL-GLUE)/Remote/Experiment/RL_remote_experiment.c
	$(CC) -c $(CFLAGS) $(RL-GLUE)/Remote/Experiment/RL_remote_experiment.c -o Build/RL_remote_experiment.o

RLnet_agent: $(RL-GLUE)/Remote/Agent/RLnet_agent.c
	$(CC) -c $(CFLAGS) $(RL-GLUE)/Remote/Agent/RLnet_agent.c -o Build/RLnet_agent.o

clean:
	rm -fr Build/*.o

tidy:
	rm -fr *.*~
	rm -fr *~
	rm -fr \#*