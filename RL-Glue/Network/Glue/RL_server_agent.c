#include <string.h> /* strlen */
#include <stdlib.h> /* calloc */

#include <RL_common.h>
#include <Network/RL_netlib.h>

static const int kAgentInit    = 1; /* agent_* start by sending one of these values */
static const int kAgentStart   = 2; /* to the client to let it know what type of    */
static const int kAgentStep    = 3; /* event to respond to                          */
static const int kAgentEnd     = 4;
static const int kAgentCleanup = 5;

static const short kDefaultPort = 4096;
static rlSocket theAgentConnection = 0;
static Action theAction = {0};

static void mallocAction(Action *theAction) {
  if (theAction != 0) {
    if (theAction->numInts > 0 && theAction->intArray == 0) {
      theAction->intArray = (int*)calloc(theAction->numInts, sizeof(int));
    }
    if (theAction->numDoubles > 0 && theAction->doubleArray == 0) {
      theAction->doubleArray = (double*)calloc(theAction->numDoubles, sizeof(double));
    }
  }
}

static void freeAction(Action *theAction) {
  if (theAction != 0) {
    free(theAction->intArray);
    free(theAction->doubleArray);

    theAction->numInts     = 0;
    theAction->numDoubles  = 0;
    theAction->intArray    = 0;
    theAction->doubleArray = 0;
  }
}

void rlSetAgentConnection(int theConnection) {
  if (theAgentConnection) {
    rlClose(theAgentConnection);
  }

  theAgentConnection = theConnection;
}

/* Send the task spec to the agent */
void agent_init(Task_specification theTaskSpec)
{
  int theTaskSpecLength = strlen(theTaskSpec);

  rlSendData(theAgentConnection, &kAgentInit, sizeof(int));
  rlSendData(theAgentConnection, &theTaskSpecLength, sizeof(int));

  if (theTaskSpecLength > 0) {
    rlSendData(theAgentConnection, &theTaskSpec, sizeof(char) * theTaskSpecLength);
  }
}

/* Send the observation to the agent, receive the action and return it */
Action agent_start(Observation theObservation)
{
  rlSendData(theAgentConnection, &kAgentStart, sizeof(int));
  rlSendADT(theAgentConnection, (RL_abstract_type*)&theObservation);

  rlRecvADTHeader(theAgentConnection, &theAction);

  /* 
     We need to allocate here because we don't know the actual sizes being sent
     from the client until now. mallocAction will not reallocate already allocated
     data. (See mallocAction above and RL_client_agent.c:onAgentStart)
  */

  mallocAction(&theAction);
  rlRecvADTBody(theAgentConnection, &theAction);

  return theAction;
}

/* Send the reward and the observation to the agent, receive the action and return it */
Action agent_step(Reward theReward, Observation theObservation)
{
  rlSendData(theAgentConnection, &kAgentStep, sizeof(int));
  rlSendData(theAgentConnection, &theReward, sizeof(Reward));
  rlSendADT(theAgentConnection, (RL_abstract_type*)&theObservation);
  rlRecvADTHeader(theAgentConnection, &theAction);
  rlRecvADTBody(theAgentConnection, &theAction);

  return theAction;
}

/* Send the final reward to the agent */
void agent_end(Reward theReward)
{  
  rlSendData(theAgentConnection, &kAgentEnd, sizeof(int));
  rlSendData(theAgentConnection, &theReward, sizeof(Reward));
}

/* Tell the agent that we're cleaning up */
void agent_cleanup()
{
  rlSendData(theAgentConnection, &kAgentCleanup, sizeof(int));
  freeAction(&theAction);
}
