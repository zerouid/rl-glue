bin_PROGRAMS = rlglue
rlglue_SOURCES = src/RL_network.c src/RL_network_experiment.c src/RL_server_environment.c src/RL_network_agent.c src/RL_network_glue.c src/RL_server_experiment.c src/RL_glue.c src/RL_network_environment.c src/RL_server_agent.c Agent_common.h Environment_common.h RL_common.h Experiment_common.h RL_network.h
#This is basically a no-op suggested by: http://www.gnu.org/software/libtool/manual/automake/Libtool-Issues.html to solve a problem where because both targets build rlglue.c
rlglue_CFLAGS = $(AM_CFLAGS)


EXAMPLE_RELEASE=3:0:0
EXAMPLE_LIBRARY_VERSION=3:0:0

lib_LTLIBRARIES = librlglue.la
 
#This might break things on linux
#LIBTOOL=glibtool
 
librlglue_la_SOURCES = src/RL_direct_experiment.c src/RL_direct_environment.c src/RL_direct_agent.c src/RL_glue.c RL_common.h Agent_common.h Environment_common.h Experiment_common.h 
librlglue_la_LDFLAGS= -version-info $(EXAMPLE_LIBRARY_VERSION) -release $(EXAMPLE_RELEASE)
librlglue_la_includedir=$(includedir)/rlglue
librlglue_la_include_HEADERS = RL_common.h Agent_common.h Environment_common.h Experiment_common.h 