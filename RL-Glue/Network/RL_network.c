#include <stdio.h>
#include <Network/RL_netlib.h> 

extern void rlSetAgentConnection(int);
extern void rlSetEnvironmentConnection(int);
extern void rlSetExperimentConnection(int);
extern int rlIsAgentConnected();
extern int rlIsEnvironmentConnected();
extern int rlIsExperimentConnected();

int rlConnectSystems() {
  int isAgentConnected       = rlIsAgentConnected();
  int isEnvironmentConnected = rlIsEnvironmentConnected(); 
  int isExperimentConnected  = rlIsExperimentConnected();
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
      break;

    case kEnvironmentConnection:
      rlSetEnvironmentConnection(theClient);
      isEnvironmentConnected = 1;
      break;

    case kExperimentConnection:
      rlSetExperimentConnection(theClient);
      isExperimentConnected = 1;
      theExperimentConnection = theClient;
      break;

    default:
      fprintf(stderr, "RL_network.c: Unknown Connection Type!\n");
      break;
    };
  }

  rlClose(theServer);

  return theExperimentConnection;
}
