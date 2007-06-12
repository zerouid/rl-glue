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
CC      = gcc
CFLAGS  = -I$(RL-GLUE)/ -ansi -pedantic -Wall
LDFLAGS =

# Uncomment next line to use network agent
#AGENT_NET       = yes
#ENVIRONMENT_NET = yes
#EXPERIMENT_NET  = yes

# ==================================== Magic Below ===========================================
GLUE_AGENT        = RL_direct_agent.o RL_agent.o
GLUE_ENVIRONMENT  = RL_direct_environment.o RL_environment.o 
GLUE_EXPERIMENT   = RL_direct_experiment.o RL_experiment.o

ifdef AGENT_NET
GLUE_AGENT        = RL_server_agent.o
USE_NETWORK       = yes
endif

ifdef ENVIRONMENT_NET
GLUE_ENVIRONMENT   = RL_server_environment.o 
USE_NETWORK        = yes
endif

ifdef EXPERIMENT_NET
GLUE_EXPERIMENT   = RL_server_experiment.o
USE_NETWORK       = yes
endif

ifdef USE_NETWORK
NETWORK_OBJECT = RL_network.o
NETLIB_OBJECT  = RL_netlib.o
else
NETWORK_OBJECT =
NETLIB_OBJECT  =  
endif

all: RL_glue RL_agent RL_environment RL_experiment

# ----------- Compile And Link RL-Glue 2.0 (Agent/Environment/Experiment Network) -----------
RL_glue: RL_interface.o $(NETWORK_OBJECT) $(NETLIB_OBJECT) $(GLUE_EXPERIMENT) $(GLUE_ENVIRONMENT) $(GLUE_AGENT) 
	$(CC) $(LDFLAGS) -o $@ Build/RL_interface.o Build/$(NETWORK_OBJECT) Build/$(NETLIB_OBJECT) Build/$(GLUE_EXPERIMENT) Build/$(GLUE_ENVIRONMENT) Build/$(GLUE_AGENT) 
# -------------------------------------------------------------------------------------------

# ------------------------- Compile And Link the Agent (Network) ----------------------------
ifdef AGENT_NET
RL_agent: RL_agent.o RL_client_agent.o RL_netlib.o
	$(CC) $(LDFLAGS) -o $@ Build/RL_agent.o Build/RL_client_agent.o Build/RL_netlib.o
endif

RL_agent.o: $(AGENTC) $(AGENTH)
	$(CC) $(CFLAGS) -c $< -o Build/$@
# -------------------------------------------------------------------------------------------

# ---------------------- Compile And Link the Environment (Network) -------------------------
ifdef ENVIRONMENT_NET
RL_environment: RL_environment.o RL_client_environment.o RL_netlib.o
	$(CC) $(LDFLAGS) -o $@ Build/RL_environment.o Build/RL_client_environment.o Build/RL_netlib.o
endif

RL_environment.o: $(ENVIRONMENTC) $(ENVIRONMENTH)
	$(CC) $(CFLAGS) -c $< -o Build/$@
# -------------------------------------------------------------------------------------------

# ---------------------- Compile And Link the Experiment (Network) --------------------------
ifdef EXPERIMENT_NET
RL_experiment: RL_experiment.o RL_client_experiment.o RL_netlib.o
	$(CC) $(LDFLAGS) -o $@ Build/RL_experiment.o Build/RL_client_experiment.o Build/RL_netlib.o
endif

RL_experiment.o: $(EXPERIMENTC) $(EXPERIMENTH)
	$(CC) $(CFLAGS) -c $< -o Build/$@
# -------------------------------------------------------------------------------------------

# ===========================================================================================






# ============================================================================================
#                                     RLGLUE 2.0
# ============================================================================================

# Path to RL-Glue to find RL_interface and related files.
RLGLUE_PATH = ../../RL-Glue

# Compiler flags
RLGLUE_CC      = gcc
RLGLUE_CFLAGS  = -I$(RLGLUE_PATH)/ -ansi -pedantic -Wall

# ===================================== INTERFACE ============================================
# ------------------------------- Compile The RL-Interface ----------------------------------
RL_interface.o: $(RLGLUE_PATH)/RL_interface.c $(RLGLUE_PATH)/RL_interface.h
	$(RLGLUE_CC) $(RLGLUE_CFLAGS) -c $< -o Build/$@
# -------------------------------------------------------------------------------------------


# ====================================== DIRECT =============================================
# ----------------------------------- Direct Call Objects -----------------------------------
RL_direct_glue.o: $(RLGLUE_PATH)/Direct/Glue/RL_direct_glue.c
	$(RLGLUE_CC) $(RLGLUE_CFLAGS) -c $< -o Build/$@

RL_direct_agent.o: $(RLGLUE_PATH)/Direct/Agent/RL_direct_agent.c
	$(RLGLUE_CC) $(RLGLUE_CFLAGS) -c $< -o Build/$@

RL_direct_environment.o: $(RLGLUE_PATH)/Direct/Environment/RL_direct_environment.c
	$(RLGLUE_CC) $(RLGLUE_CFLAGS) -c $< -o Build/$@

RL_direct_experiment.o: $(RLGLUE_PATH)/Direct/Experiment/RL_direct_experiment.c
	$(RLGLUE_CC) $(RLGLUE_CFLAGS) -c $< -o Build/$@
# -------------------------------------------------------------------------------------------

# ====================================== NETWORK ============================================
# -------------------------------------- Client ---------------------------------------------
RL_client_agent.o: $(RLGLUE_PATH)/Network/Agent/RL_client_agent.c
	$(RLGLUE_CC) $(RLGLUE_CFLAGS) -c $< -o Build/$@

RL_client_environment.o: $(RLGLUE_PATH)/Network/Environment/RL_client_environment.c
	$(RLGLUE_CC) $(RLGLUE_CFLAGS) -c $< -o Build/$@

RL_client_experiment.o: $(RLGLUE_PATH)/Network/Experiment/RL_client_experiment.c
	$(RLGLUE_CC) $(RLGLUE_CFLAGS) -c $< -o Build/$@
# -------------------------------------------------------------------------------------------

# -------------------------------------- Server ---------------------------------------------
RL_server_agent.o: $(RLGLUE_PATH)/Network/Glue/RL_server_agent.c
	$(RLGLUE_CC) $(RLGLUE_CFLAGS) -c $< -o Build/$@

RL_server_environment.o: $(RLGLUE_PATH)/Network/Glue/RL_server_environment.c
	$(RLGLUE_CC) $(RLGLUE_CFLAGS) -c $< -o Build/$@

RL_server_experiment.o: $(RLGLUE_PATH)/Network/Glue/RL_server_experiment.c
	$(RLGLUE_CC) $(RLGLUE_CFLAGS) -c $< -o Build/$@
# -------------------------------------------------------------------------------------------

# ------------------------------------ Network Library --------------------------------------
RL_network.o: $(RLGLUE_PATH)/Network/RL_network.c
	$(RLGLUE_CC) $(RLGLUE_CFLAGS) -c $< -o Build/$@

RL_netlib.o: $(RLGLUE_PATH)/Network/RL_netlib.c $(RLGLUE_PATH)/Network/RL_netlib.h
	$(RLGLUE_CC) $(RLGLUE_CFLAGS) -c $< -o Build/$@
# -------------------------------------------------------------------------------------------


clean:
	rm -fr Build/*.o

tidy:
	rm -fr *.*~
	rm -fr *~
	rm -fr \#*