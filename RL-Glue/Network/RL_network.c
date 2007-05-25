#include "RLnet.h"

const short kDefaultPort = 4696;

const int kAgentConnection       = 1;
const int kEnvironmentConnection = 2;
const int kExperimentConnection  = 3;

int accept_connection(rlSocket theServer) {
  int theClientType = 0;
  rlSocket theClient = rlAcceptConnection(theServer);
  rlRecvData(theClient, &theClientType, sizeof(int));
  return theClientType;
}

void wait_for_connections() {
  int isAgentConnected = 0;
  int isEnvironmentConnected = 1; 
  int isExperimentConnected = 1;
  int theClientType = 0;

  rlSocket theClient;
  rlSocket theServer = rlOpen(kDefaultPort);
  rlListen(theServer);

  while(!isAgentConnected && !isEnvironmentConnected && !isExperimentConnected) {
    theClient = rlAcceptConnection(theServer);
    rlRecvData(theClient, &theClientType, sizeof(int));

    switch(theClientType) {
    case kAgentConnection:
      isAgentConnected = 1;
      break;

    case kEnvironmentConnection:
      isEnvironmentConnected = 1;
      break;

    case kExperimentConnection:
      isExperimentConnected = 1;
      break;

    default:
      break;
    };
  }

  rlClose(theServer);
}
