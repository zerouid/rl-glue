#include <stdio.h> /* fprintf */
#include <Network/RL_netlib.h> 

extern void rlSetAgentConnection(int);
extern void rlSetEnvironmentConnection(int);
/* extern void rlSetExperimentConnection(int); */

int rlConnectSystems() {
  int isAgentConnected       = 0;
  int isEnvironmentConnected = 0; 
  int isExperimentConnected  = 0;
  int theClientType = 0;
  int theClient = 0;
  int theServer = 0;

  rlSocket theExperimentConnection = 0;

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
      theExperimentConnection = theClient;
      fprintf(stderr, "RL_network.c: Environment Connected!\n");
      break;

    case kExperimentConnection:
      /* rlSetExperimentConnection(theClient); */
      isExperimentConnected = 1;
      fprintf(stderr, "RL_network.c: Experiment Connected!\n");
      break;

    default:
      fprintf(stderr, "RL_network.c: Unknown Connection Type!\n");
      break;
    };
  }

  rlClose(theServer);
  fprintf(stderr, "Systems Connected. Go Team Venture!\n");

  return theExperimentConnection;
}
