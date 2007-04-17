.PHONY: all clean clean-irix

Experiment = Experiment.c 
AGENTV = mineAgent.cpp 
ENVV = mines.cpp 
COMMON = mines_common.h 

CPPFLAGS=-O3 -Wall
CXX=g++

all:  ../../RL-Glue/RLcommon.h  Build/agent.o Build/env.o Build/main.o Build/parser.o Build/RL_Interface.o
	${CXX} ${CPPFLAGS} Build/env.o Build/RL_Interface.o Build/agent.o Build/main.o Build/parser.o -o RL_main 

../../RL-Glue/RLcommon.h: ${COMMON}
	cp ${COMMON} ../../RL-Glue/RLcommon.h
 
Build/RL_Interface.o: ../../RL-Glue/RL_Interface.c 
	${CXX} ${CPPFLAGS} -g -o Build/RL_Interface.o -c ../../RL-Glue/RL_Interface.c

Build/main.o: $(Experiment)
	${CXX}  ${CPPFLAGS} -I ../../RL-Glue/ -I Stats/ -g -o Build/main.o -c $(Experiment) 

Build/agent.o: $(AGENTV) 
	${CXX} ${CPPFLAGS} -I ../../RL-Glue/ -I Utils/ -g -o Build/agent.o -c $(AGENTV) 

Build/env.o: $(ENVV) 
	${CXX} ${CPPFLAGS} -I ../../RL-Glue/ -g -o Build/env.o -c $(ENVV) 

Build/parser.o: Utils/C_agent_parser.cpp
	${CXX} ${CPPFLAGS} -g -c -o Build/parser.o Utils/C_agent_parser.cpp

clean: 
	-rm -rf Build/*.o RL_main core* ../../RL-Glue/RLcommon.h 
clean-irix:
	rm -rf Build/*.o RL_main core* ../../RL-Glue/RLcommon.h