#include <assert.h> /* assert  */
#include <stdlib.h> /* calloc */
#include <stdio.h>  /* fprintf */
#include <unistd.h> /* sleep   */

#include <RL_common.h>
#include <Network/RL_netlib.h>

/* Provide forward declaration of agent interface */
extern void   agent_init(Task_specification task_spec);
extern Action agent_start(Observation o);
extern Action agent_step(Reward r, Observation o);
extern void   agent_end(Reward r);
extern void   agent_cleanup();

static const char* kUnknownMessage = "Unknown Message: %d\n";

static char* theTaskSpec = 0;
static Observation theObservation = {0};
static int isObservationAllocated = 0;

static rlSocket waitForConnection(const char* address, const short port, const int retryTimeout) {
  rlSocket theConnection = 0;
  int isConnected = -1;

  while(isConnected == -1) {
    theConnection = rlOpen(port);
    assert(rlIsValidSocket(theConnection));
    isConnected = rlConnect(theConnection, address, port);
    if (isConnected == -1) { 
      rlClose(theConnection);
      sleep(retryTimeout);
    }
  }

  return theConnection;
}

static void onAgentInit(rlSocket theConnection)
{
  int theTaskSpecLength = 0;

  rlRecvData(theConnection, &theTaskSpecLength, sizeof(int));
  if (theTaskSpecLength > 0) {
    theTaskSpec = (char*)calloc(theTaskSpecLength, sizeof(char));
    rlRecvData(theConnection, theTaskSpec, sizeof(char) * theTaskSpecLength);
  }

  agent_init(theTaskSpec);
}

static void onAgentStart(rlSocket theConnection)
{
  Action theAction = {0};

  rlRecvADTHeader(theConnection, (RL_abstract_type*)&theObservation);

  if (!isObservationAllocated) {
    rlAllocADT(&theObservation);
    isObservationAllocated = 1;
  }

  rlRecvADTBody(theConnection, (RL_abstract_type*)&theObservation);

  theAction = agent_start(theObservation);
  rlSendADT(theConnection, (RL_abstract_type*)&theAction);
}

static void onAgentStep(rlSocket theConnection)
{
  Reward theReward = 0;
  Action theAction = {0};

  rlRecvData(theConnection, &theReward, sizeof(Reward));
  rlRecvADTHeader(theConnection, (RL_abstract_type*)&theObservation);
  rlRecvADTBody(theConnection, (RL_abstract_type*)&theObservation);

  theAction = agent_step(theReward, theObservation);
  rlSendADT(theConnection, (RL_abstract_type*)&theAction);
}

static void onAgentEnd(rlSocket theConnection)
{
  Reward theReward = 0;

  rlRecvData(theConnection, &theReward, sizeof(Reward));
  agent_end(theReward);
}

static void onAgentCleanup(rlSocket theConnection)
{
  agent_cleanup();

  rlFreeADT(&theObservation);
  isObservationAllocated = 0;

  free(theTaskSpec);
  theTaskSpec = 0;
}

static void runAgentEventLoop(rlSocket theConnection)
{
  int agentState = 0;

  do { 
    rlRecvData(theConnection, &agentState, sizeof(int));

    switch(agentState) {
    case kAgentInit:
      onAgentInit(theConnection);
      break;

    case kAgentStart:
      onAgentStart(theConnection);
      break;

    case kAgentStep:
      onAgentStep(theConnection);
      break;

    case kAgentEnd:
      onAgentEnd(theConnection);
      break;

    case kAgentCleanup:
      onAgentCleanup(theConnection);
      break;

    default:
      fprintf(stderr, kUnknownMessage, agentState);
      break;
    };
  } while (agentState != kAgentCleanup);
}

int main(int argc, char** argv) {
  int theConnectionType = kAgentConnection;
  rlSocket theConnection = 0;

  while(1) {
    theConnection = waitForConnection(kLocalHost, kDefaultPort, kRetryTimeout);
    /* we need to tell RL-Glue what type of object is connecting */
    rlSendData(theConnection, &theConnectionType, sizeof(int)); 
    runAgentEventLoop(theConnection);
    rlClose(theConnection);
  }

  return 0;
}
