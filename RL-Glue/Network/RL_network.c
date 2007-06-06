#include <Network/RL_netlib.h>

#include <stdio.h>

static const short kDefaultPort = 4096;

enum { 
  kAgentConnection       = 1, 
  kEnvironmentConnection = 2, 
  kExperimentConnection  = 3 
};

extern void rlSetAgentConnection(int);
extern void rlSetEnvironmentConnection(int);
extern void rlSetExperimentConnection(int);

void rlConnectSystems() {
  int isAgentConnected       = 0;
  int isEnvironmentConnected = 1; 
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
      break;

    case kEnvironmentConnection:
      rlSetEnvironmentConnection(theClient);
      isEnvironmentConnected = 1;
      break;

    case kExperimentConnection:
      rlSetExperimentConnection(theClient);
      isExperimentConnected = 1;
      break;

    default:
      break;
    };
  }

  rlClose(theServer);
}
