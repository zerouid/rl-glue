bin_PROGRAMS = rl_glue
rl_glue_SOURCES = src/RL_network.c src/RL_network_experiment.c src/RL_server_environment.c src/RL_network_agent.c src/RL_network_glue.c src/RL_server_experiment.c src/RL_glue.c src/RL_network_environment.c src/RL_server_agent.c rlglue/Agent_common.h rlglue/Environment_common.h rlglue/RL_common.h rlglue/Experiment_common.h src/RL_network.h
#This is basically a no-op suggested by: http://www.gnu.org/software/libtool/manual/automake/Libtool-Issues.html to solve a problem where because both targets build rlglue.c
rl_glue_CFLAGS = $(AM_CFLAGS)


GLUE_RELEASE=3:0:0
GLUE_VERSION=3:0:0

lib_LTLIBRARIES = librlglue.la
 

librlglue_la_SOURCES = src/RL_direct_experiment.c src/RL_direct_environment.c src/RL_direct_agent.c src/RL_glue.c rlglue/RL_common.h rlglue/Agent_common.h rlglue/Environment_common.h rlglue/Experiment_common.h 

librlglue_la_LDFLAGS= -version-info $(GLUE_VERSION) -release $(GLUE_RELEASE)
librlglue_la_includedir=$(includedir)/rlglue
librlglue_la_include_HEADERS = rlglue/RL_common.h rlglue/Agent_common.h rlglue/Environment_common.h  rlglue/Experiment_common.h 