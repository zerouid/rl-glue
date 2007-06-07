#include <Network/RL_netlib.h>
#include <stdio.h>

extern void rlSetAgentConnection(int);
extern void rlSetEnvironmentConnection(int);
extern void rlSetExperimentConnection(int);

void rlConnectSystems() {
  int isAgentConnected       = 0;
  int isEnvironmentConnected = 0; 
  int isExperimentConnected  = 1;
  int theClientType = 0;
  int theClient = 0;
  int theServer = 0;
  theServer = rlOpen(kDefaultPort);
  rlListen(theServer, kDefaultPort);

  while(!isAgentConnected || !isEnvironmentConnected || !isExperimentConnected) {
    theClient = rlAcceptConnection(theServer);
    rlRecvData(theClient, &theClientType, sizeof(int));

    switch(theClientType) {
    case kAgentConnection:
      rlSetAgentConnection(theClient);
      isAgentConnected = 1;
      fprintf(stderr, "RL_network.c: Agent Connected!\n");
      break;

    case kEnvironmentConnection:
      rlSetEnvironmentConnection(theClient);
      isEnvironmentConnected = 1;
      fprintf(stderr, "RL_network.c: Environment Connected!\n");
      break;

    case kExperimentConnection:
      rlSetExperimentConnection(theClient);
      isExperimentConnected = 1;
      fprintf(stderr, "RL_network.c: Experiment Connected!\n");
      break;

    default:
      break;
    };
  }

  rlClose(theServer);
  fprintf(stderr, "Systems Connected. Go Team Venture!\n");
}
