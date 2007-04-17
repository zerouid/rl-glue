CC = g++
RL-Glue = ../../RL-Glue
Parser = $(RL-Glue)/Parser/cpp
Sockets = $(RL-Glue)/Sockets/cpp

# We need to search the RL-Glue directory RL-Interface.h 
# the Parser/cpp directory for the task string parser
# and for the environment directory for the types header
CFLAGS = -I$(RL-Glue)/ -I$(RL-Glue)/Parser/cpp/ -I./Env/

# This file contains the type definition of observations and actions. Its path should be included above in the CFLAGS
TYPES_HEADER = "<mines_types.h>"

all: Glue Agent Environment Benchmark
	$(CC) -o RL_main Build/RL_Interface.o Build/mineAgent.o Build/mines.o Build/RL_parser.o Build/main.o

Glue: RL_Interface.o
Agent: mineAgent.o RL_parser.o RLnet.o RLnet_agent.o
	$(CC) -o RLnet_agent Build/mineAgent.o Build/RLnet_agent.o Build/RLnet.o Build/RL_parser.o

Environment: mines.o RL_parser.o
Benchmark: main.o

#FIXME RL_Interface.c should be RL_Interface.cpp
RL_Interface.o: $(RL-Glue)/RL_Interface.c $(RL-Glue)/RL_Interface.h
	$(CC) -c $(CFLAGS) -DTYPES_HEADER=$(TYPES_HEADER) $< -o Build/$@

RL_parser.o: $(Parser)/RL_parser.cpp
	$(CC) -c $(CFLAGS) -DTYPES_HEADER=$(TYPES_HEADER) $< -o Build/$@

mineAgent.o: ./Agent/mineAgent.cpp ./Agent/mineAgent.h 
	$(CC) -c $(CFLAGS) -DTYPES_HEADER=$(TYPES_HEADER) $< -o Build/$@

RLnet.o: ./$(Sockets)/RLnet.cpp ./$(Sockets)/RLnet.h
	$(CC) -c $(CFLAGS) -DTYPES_HEADER=$(TYPES_HEADER) $< -o Build/$@

RLnet_agent.o: ./$(Sockets)/RLnet_agent.cpp
	$(CC) -c $(CFLAGS) -DTYPES_HEADER=$(TYPES_HEADER) $< -o Build/$@

mines.o: ./Env/mines.c ./Env/mines.h
	$(CC) -c $(CFLAGS) -DTYPES_HEADER=$(TYPES_HEADER) $< -o Build/$@

main.o: ./Benchmark/main.c
	$(CC) -c $(CFLAGS) -DTYPES_HEADER=$(TYPES_HEADER) $< -o Build/$@

clean:
	rm -fr Build/*.o
	rm -fr RL_main
	rm -fr RLnet_agent

tidy:
	rm -fr *.*~
	rm -fr *~
	rm -fr \#*
