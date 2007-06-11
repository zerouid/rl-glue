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

# ==================================== User Code ===========================================
all: RL_glue RL_agent RL_environment RL_experiment

# ----------------------- Compile And Link RL-Glue 2.0 (Agent/Environment Network) ----------------------
RL_glue: RL_interface.o RL_netlib.o RL_server_experiment.o RL_server_environment.o RL_server_agent.o 
	$(CC) $(LDFLAGS) -o $@ Build/RL_interface.o Build/RL_netlib.o Build/RL_server_experiment.o Build/RL_server_environment.o Build/RL_server_agent.o 
# -------------------------------------------------------------------------------------------

# ------------------------- Compile And Link the Agent (Network) ----------------------------
RL_agent: RL_agent.o RL_client_agent.o RL_netlib.o
	$(CC) $(LDFLAGS) -o $@ Build/RL_agent.o Build/RL_client_agent.o Build/RL_netlib.o

RL_agent.o: $(AGENTC) $(AGENTH)
	$(CC) $(CFLAGS) -c $< -o Build/$@
# -------------------------------------------------------------------------------------------

# ---------------------- Compile And Link the Environment (Network) -------------------------
RL_environment: RL_environment.o RL_client_environment.o RL_netlib.o
	$(CC) $(LDFLAGS) -o $@ Build/RL_environment.o Build/RL_client_environment.o Build/RL_netlib.o

RL_environment.o: $(ENVIRONMENTC) $(ENVIRONMENTH)
	$(CC) $(CFLAGS) -c $< -o Build/$@
# -------------------------------------------------------------------------------------------

# ---------------------- Compile And Link the Experiment (Network) --------------------------
RL_experiment: RL_experiment.o RL_client_experiment.o RL_netlib.o
	$(CC) $(LDFLAGS) -o $@ Build/RL_experiment.o Build/RL_client_experiment.o Build/RL_netlib.o
	
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
RL_netlib.o: $(RLGLUE_PATH)/Network/RL_netlib.c $(RLGLUE_PATH)/Network/RL_netlib.h
	$(RLGLUE_CC) $(RLGLUE_CFLAGS) -c $< -o Build/$@
# -------------------------------------------------------------------------------------------


clean:
	rm -fr Build/*.o

tidy:
	rm -fr *.*~
	rm -fr *~
	rm -fr \#*