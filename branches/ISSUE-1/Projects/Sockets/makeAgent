CC = g++
RL-Glue = ../../RL-Glue

# We need to search the RL-Glue directory RL-Interface.h 
# the Parser/cpp directory for the task string parser
# and for the environment directory for the types header
CFLAGS = -I$(RL-Glue)/ -I$(RL-Glue)/Parser/cpp/ -ansi -pedantic -Wall

all: Glue Agent Environment Benchmark
	$(CC) -o RL_main Build/RL_Interface.o Build/ZeroAgent.o Build/mines.o Build/RL_parser.o Build/main.o

Glue: RL_Interface.o
Agent: ZeroAgent.o
Environment: mines.o RL_parser.o
Benchmark: main.o

#FIXME RL_Interface.c should be RL_Interface.c
RL_Interface.o: $(RL-Glue)/RL_Interface.c $(RL-Glue)/RL_Interface.h
	$(CC) -c $(CFLAGS) $< -o Build/RL_Interface.o

ZeroAgent.o: ./Agent/ZeroAgent.c ./Agent/ZeroAgent.h
	$(CC) -c $(CFLAGS) $< -o Build/ZeroAgent.o

RL_parser.o: $(RL-Glue)/Parser/cpp/RL_parser.cpp $(RL-Glue)/Parser/cpp/RL_parser.h
	$(CC) -c $(CFLAGS) $< -o Build/RL_parser.o

mines.o: ./Env/mines.c ./Env/mines.h
	$(CC) -c $(CFLAGS) $< -o Build/mines.o

main.o: ./Benchmark/main.c
	$(CC) -c $(CFLAGS) $< -o Build/main.o

clean:
	rm -fr Build/*.o
	rm -fr RL_main

tidy:
	rm -fr *.*~
	rm -fr *~
	rm -fr \#*
