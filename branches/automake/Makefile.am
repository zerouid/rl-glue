bin_PROGRAMS = rlglue
rlglue_SOURCES = src/RL_network.c src/RL_network_experiment.c src/RL_server_environment.c src/RL_network_agent.c src/RL_network_glue.c src/RL_server_experiment.c src/RL_glue.c src/RL_network_environment.c src/RL_server_agent.c src/rlglue/Agent_common.h src/rlglue/Environment_common.h src/rlglue/RL_common.h src/rlglue/Experiment_common.h src/RL_network.h
#This is basically a no-op suggested by: http://www.gnu.org/software/libtool/manual/automake/Libtool-Issues.html to solve a problem where because both targets build rlglue.c
rlglue_CFLAGS = $(AM_CFLAGS)
INCLUDES = -Isrc

GLUE_RELEASE=3:0:0
GLUE_VERSION=3:0:0

lib_LTLIBRARIES = librlglue.la
 

librlglue_la_SOURCES = src/RL_direct_experiment.c src/RL_direct_environment.c src/RL_direct_agent.c src/RL_glue.c src/rlglue/RL_common.h src/rlglue/Agent_common.h src/rlglue/Environment_common.h src/rlglue/Experiment_common.h 

librlglue_la_LDFLAGS= -version-info $(GLUE_VERSION) -release $(GLUE_RELEASE)
librlglue_la_includedir=$(includedir)/rlglue
librlglue_la_include_HEADERS = src/rlglue/RL_common.h src/rlglue/Agent_common.h src/rlglue/Environment_common.h  src/rlglue/Experiment_common.h 