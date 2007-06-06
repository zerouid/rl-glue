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

# Objects we need to link against for a network compilation
NETWORK_OBJECTS = Build/RL_network.o Build/RL_netlib.o

# Build everything. Just as an example.
all: agent environment experiment glue direct network
	$(CC) -o RL_main Build/environment.o Build/experiment.o Build/RL_interface.o Build/RL_server_agent.o $(NETWORK_OBJECTS)
	$(CC) -o RL_agent Build/agent.o Build/RL_netlib.o Build/RL_client_agent.o

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

network: RL_network RL_netlib RL_server_agent

RL_network: $(RL-GLUE)/Network/RL_network.c
	$(CC) -c $(CFLAGS) $(RL-GLUE)/Network/RL_network.c -o Build/RL_network.o

RL_netlib: $(RL-GLUE)/Network/RL_netlib.c
	$(CC) -c $(CFLAGS) $(RL-GLUE)/Network/RL_netlib.c -o Build/RL_netlib.o

RL_server_agent: $(RL-GLUE)/Network/Glue/RL_server_agent.c
	$(CC) -c $(CFLAGS) $(RL-GLUE)/Network/Glue/RL_server_agent.c -o Build/RL_server_agent.o

clean:
	rm -fr Build/*.o

tidy:
	rm -fr *.*~
	rm -fr *~
	rm -fr \#*