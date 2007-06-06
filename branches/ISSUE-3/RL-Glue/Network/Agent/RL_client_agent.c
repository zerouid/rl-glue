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

/* RL-Glue needs to know what type of object is trying to connect. */
static const int kAgentConnection = 1;

static const int kAgentInit    = 1; /* agent_* start by sending one of these values */
static const int kAgentStart   = 2; /* to the client to let it know what type of    */
static const int kAgentStep    = 3; /* event to respond to                          */
static const int kAgentEnd     = 4;
static const int kAgentCleanup = 5;

static const char* kUnknownMessage = "Unknown Message: %d\n";

static const char* kLocalHost   = "127.0.0.1";
static const short kDefaultPort = 4096;
static const int kRetryTimeout  = 10;

static char* theTaskSpec = 0;
static Observation theObservation = {0};

static void mallocObservation(Observation *theObservation) {
  if (theObservation != 0) {
    if (theObservation->numInts > 0 && theObservation->intArray == 0) {
      theObservation->intArray = (int*)calloc(theObservation->numInts, sizeof(int));
    }
    if (theObservation->numDoubles > 0 && theObservation->doubleArray == 0) {
      theObservation->doubleArray = (double*)calloc(theObservation->numDoubles, sizeof(double));
    }
  }
}

static void freeObservation(Observation *theObservation) {
  if (theObservation != 0) {
    free(theObservation->intArray);
    free(theObservation->doubleArray);

    theObservation->numInts     = 0;
    theObservation->numDoubles  = 0;
    theObservation->intArray    = 0;
    theObservation->doubleArray = 0;
  }
}

static rlSocket waitForConnection(const char* address, const short port, const int retryTimeout) {
  rlSocket theConnection = 0;
  int isConnected = -1;

  while(isConnected == -1) {
    theConnection = rlOpen(port);
    assert(rlIsValidSocket(theConnection));
    isConnected = rlConnect(theConnection, kLocalHost, kDefaultPort);
    if (isConnected == -1) { 
      rlClose(theConnection);
      sleep(retryTimeout);
    }
}

static void onAgentInit(rlSocket theConnection)
{
  int theTaskSpecLength = 0;
  rlRecvData(theConnection, &theTaskSpecLength, sizeof(int));

  if (theTaskSpecLength > 0) {
    theTaskSpec = (char*)calloc(theTaskSpecLength, 1);
    rlRecvData(theConnection, theTaskSpec, sizeof(char) * theTaskSpecLength);
  }

  agent_init(theTaskSpec);
}

static void onAgentStart(rlSocket theConnection)
{
  Action theAction = {0};

  rlRecvADTHeader(theConnection, (RL_abstract_type*)&theObservation);
  
  /* 
     We need to allocate here because we don't know the actual sizes being sent
     from the server until now. mallocAction will not reallocate already allocated
     data. (See mallocObservation above and RL_server_agent.c:agent_start)
  */

  mallocObservation(&theObservation);
  rlRecvADTBody(theConnection, (RL_abstract_type*)&theObservation);

  theAction = agent_start(theObservation);
  rlSendADT(theConnection, (RL_abstract_type*)&theAction);
}

static void onAgentStep(rlSocket theConnection)
{
  Reward theReward = 0;
  Action theAction = 0;

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

  free(theTaskSpec);
  theTaskSpec = 0;

  freeObservation(&theObservation);
}

static void runAgentEventLoop(rlSocket theConnection)
{
  int agentState = 0;

  do
  { 
    rlRecvData(theConnection, agentState, sizeof(int));

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

int main(int argc, char** argv)
{
  rlSocket theConnection = 0;

  while(1) {
    theConnection = waitForConnection(kLocalHost, kDefaultPort, kRetryTimeout);
    /* we need to tell RL-Glue what type of object is connecting */
    rlSendData(theConnection, kAgentConnection, sizeof(int)); 
    runAgentEventLoop(theConnection);
    rlClose(theConnection);
  }

  return 0;
}
