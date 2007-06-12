AGENT       = ./Agent/ZeroAgent.c 
ENVIRONMENT = ./Environment/mines.c
EXPERIMENT  = ./Experiment/experiment.c

# Path to RL-Glue to find RL_interface and related files.
RL-GLUE = ../../RL-Glue

# Compiler flags
CC         = gcc
CFLAGS     = -I$(RL-GLUE)/ -ansi -pedantic -Wall
LDFLAGS    =
BUILD_PATH = Build/

# Uncomment next line to use network agent
AGENT_NET       = yes
ENVIRONMENT_NET = yes
EXPERIMENT_NET  = yes


# ============================================================================================
#                                     RLGLUE 2.0
# ============================================================================================
GLUE_AGENT        = RL_direct_agent.o RL_agent.o
GLUE_ENVIRONMENT  = RL_direct_environment.o RL_environment.o 
GLUE_EXPERIMENT   = RL_direct_experiment.o RL_experiment.o


AGENT_TARGET      = 
ifdef AGENT_NET
GLUE_AGENT        = RL_server_agent.o
AGENT_TARGET      = RL_agent
USE_NETWORK       = yes
endif

ENVIRONMENT_TARGET =
ifdef ENVIRONMENT_NET
GLUE_ENVIRONMENT   = RL_server_environment.o 
ENVIRONMENT_TARGET = RL_environment
USE_NETWORK        = yes
endif

EXPERIMENT_TARGET = 
ifdef EXPERIMENT_NET
GLUE_EXPERIMENT   = RL_server_experiment.o
EXPERIMENT_TARGET = RL_experiment
USE_NETWORK       = yes
endif

ifdef USE_NETWORK
NETWORK_OBJECT = RL_network.o
NETLIB_OBJECT  = RL_netlib.o
else
NETWORK_OBJECT =
NETLIB_OBJECT  =  
endif


all: RL_glue $(AGENT_TARGET) $(ENVIRONMENT_TARGET) $(EXPERIMENT_TARGET)

# ------------------------------- Compile And Link RL-Glue 2.0 ------------------------------
RL_glue: RL_interface.o $(NETWORK_OBJECT) $(NETLIB_OBJECT) $(GLUE_EXPERIMENT) $(GLUE_ENVIRONMENT) $(GLUE_AGENT) 
	$(CC) $(LDFLAGS) -o $@  $(addprefix $(BUILD_PATH), RL_interface.o)      \
				$(addprefix $(BUILD_PATH), $(NETWORK_OBJECT))   \
				$(addprefix $(BUILD_PATH), $(NETLIB_OBJECT))    \
				$(addprefix $(BUILD_PATH), $(GLUE_EXPERIMENT))  \
				$(addprefix $(BUILD_PATH), $(GLUE_ENVIRONMENT)) \
				$(addprefix $(BUILD_PATH), $(GLUE_AGENT))
# -------------------------------------------------------------------------------------------

# ------------------------------ Compile And Link the Agent ---------------------------------
RL_agent: RL_agent.o RL_client_agent.o RL_netlib.o
	$(CC) $(LDFLAGS) -o $@  $(addprefix $(BUILD_PATH), RL_agent.o) $(addprefix $(BUILD_PATH), RL_client_agent.o) $(addprefix $(BUILD_PATH), RL_netlib.o)

RL_agent.o: $(AGENT)
	$(CC) $(CFLAGS) -c $< -o $(addprefix $(BUILD_PATH), $@)
# -------------------------------------------------------------------------------------------

# --------------------------- Compile And Link the Environment ------------------------------
RL_environment: RL_environment.o RL_client_environment.o RL_netlib.o
	$(CC) $(LDFLAGS) -o $@  $(addprefix $(BUILD_PATH), RL_environment.o) $(addprefix $(BUILD_PATH), RL_client_environment.o) $(addprefix $(BUILD_PATH), RL_netlib.o)

RL_environment.o: $(ENVIRONMENT)
	$(CC) $(CFLAGS) -c $< -o $(addprefix $(BUILD_PATH), $@)
# -------------------------------------------------------------------------------------------

# --------------------------- Compile And Link the Experiment -------------------------------
RL_experiment: RL_experiment.o RL_client_experiment.o RL_netlib.o
	$(CC) $(LDFLAGS) -o $@  $(addprefix $(BUILD_PATH), RL_experiment.o) $(addprefix $(BUILD_PATH), RL_client_experiment.o) $(addprefix $(BUILD_PATH), RL_netlib.o)

RL_experiment.o: $(EXPERIMENT)
	$(CC) $(CFLAGS) -c $< -o $(addprefix $(BUILD_PATH), $@)
# -------------------------------------------------------------------------------------------


# ===================================== INTERFACE ============================================
# ------------------------------- Compile The RL-Interface ----------------------------------
RL_interface.o: $(RL-GLUE)/RL_interface.c $(RL-GLUE)/RL_interface.h
	$(CC) $(CFLAGS) -c $< -o $(addprefix $(BUILD_PATH), $@)
# -------------------------------------------------------------------------------------------


# ====================================== DIRECT =============================================
# ----------------------------------- Direct Call Objects -----------------------------------
RL_direct_glue.o: $(RL-GLUE)/Direct/Glue/RL_direct_glue.c
	$(CC) $(CFLAGS) -c $< -o $(addprefix $(BUILD_PATH), $@)

RL_direct_agent.o: $(RL-GLUE)/Direct/Agent/RL_direct_agent.c
	$(CC) $(CFLAGS) -c $< -o $(addprefix $(BUILD_PATH), $@)

RL_direct_environment.o: $(RL-GLUE)/Direct/Environment/RL_direct_environment.c
	$(CC) $(CFLAGS) -c $< -o $(addprefix $(BUILD_PATH), $@)

RL_direct_experiment.o: $(RL-GLUE)/Direct/Experiment/RL_direct_experiment.c
	$(CC) $(CFLAGS) -c $< -o $(addprefix $(BUILD_PATH), $@)
# -------------------------------------------------------------------------------------------

# ====================================== NETWORK ============================================
# -------------------------------------- Client ---------------------------------------------
RL_client_agent.o: $(RL-GLUE)/Network/Agent/RL_client_agent.c
	$(CC) $(CFLAGS) -c $< -o $(addprefix $(BUILD_PATH), $@)

RL_client_environment.o: $(RL-GLUE)/Network/Environment/RL_client_environment.c
	$(CC) $(CFLAGS) -c $< -o $(addprefix $(BUILD_PATH), $@)

RL_client_experiment.o: $(RL-GLUE)/Network/Experiment/RL_client_experiment.c
	$(CC) $(CFLAGS) -c $< -o $(addprefix $(BUILD_PATH), $@)
# -------------------------------------------------------------------------------------------

# -------------------------------------- Server ---------------------------------------------
RL_server_agent.o: $(RL-GLUE)/Network/Glue/RL_server_agent.c
	$(CC) $(CFLAGS) -c $< -o $(addprefix $(BUILD_PATH), $@)

RL_server_environment.o: $(RL-GLUE)/Network/Glue/RL_server_environment.c
	$(CC) $(CFLAGS) -c $< -o $(addprefix $(BUILD_PATH), $@)

RL_server_experiment.o: $(RL-GLUE)/Network/Glue/RL_server_experiment.c
	$(CC) $(CFLAGS) -c $< -o $(addprefix $(BUILD_PATH), $@)
# -------------------------------------------------------------------------------------------

# ------------------------------------ Network Library --------------------------------------
RL_network.o: $(RL-GLUE)/Network/RL_network.c
	$(CC) $(CFLAGS) -c $< -o $(addprefix $(BUILD_PATH), $@)

RL_netlib.o: $(RL-GLUE)/Network/RL_netlib.c $(RL-GLUE)/Network/RL_netlib.h
	$(CC) $(CFLAGS) -c $< -o $(addprefix $(BUILD_PATH), $@)
# -------------------------------------------------------------------------------------------


clean:
	rm -fr $(addprefix $(BUILD_PATH), *.o)
	rm -fr RL_main
	rm -fr RL_glue
	rm -fr RL_agent
	rm -fr RL_environment
	rm -fr RL_experiment

tidy:
	rm -fr *.*~
	rm -fr *~
	rm -fr \#*