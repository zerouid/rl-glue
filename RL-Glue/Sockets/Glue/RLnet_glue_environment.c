#ifdef NETWORK_DEBUG
#define RLNET_DEBUG(x) x
#else
#define RLNET_DEBUG(x)
#endif

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <RLcommon.h>
#include <RLnet/RLnet.h>

/* Could be culled, used only for debugging */
#include <stdio.h>

rlSocket theConnection;
Action theAction;

const char* kInit = "init";
const char* kStart= "start";
const char* kStep = "step";
const char* kEnd  = "end";
const char* kTerm = "term";
const char* kCleanup = "cleanup";

void print_reward(Reward theReward)
{
  fprintf(stderr, "GLUE SENT: theReward %f\n", theReward);
}

void print_action_header(Action theAction)
{
  fprintf(stderr, "GLUE RECV: theAction.numInts = %d\n", theAction.numInts);
  fprintf(stderr, "GLUE RECV: theAction.numDoubles = %d\n", theAction.numDoubles);
}


void print_observation_header(Observation theObservation)
{
  fprintf(stderr, "GLUE SENT: theObservation.numInts = %d\n", theObservation.numInts);
  fprintf(stderr, "GLUE SENT: theObservation.numDoubles = %d\n", theObservation.numDoubles);
}

void send_msg(rlSocket theSocket, const char* theMessage)
{
  char send_buffer[8] = {0};
  strncpy(send_buffer, theMessage, 8);
  rlSendData(theSocket, send_buffer, 8);

  RLNET_DEBUG( fprintf(stderr, "GLUE SENT: %s\n", send_buffer); )
}

void agent_init(Task_specification task_spec)
{
  int taskSpecLength = 0;
  rlSocket theServer = rlOpen(4096);
  assert(rlIsValidSocket(theServer));
  assert(rlListen(theServer) >= 0);
  theConnection = rlAcceptConnection(theServer);
  rlClose(theServer);

  assert(rlIsValidSocket(theConnection));

  send_msg(theConnection, kInit);

  /* Warning, strlen, could be used as buffer-overrun exploit */
  taskSpecLength = strlen(task_spec)+1;
  rlSendData(theConnection, &taskSpecLength, sizeof(int));
  rlSendData(theConnection, task_spec, taskSpecLength);
}

Action agent_start(Observation theObservation)
{
  send_msg(theConnection, kStart);

  rlSendObservation(theConnection, theObservation);
  RLNET_DEBUG( print_observation_header(theObservation); )

  rlRecvActionHeader(theConnection, &theAction);
  RLNET_DEBUG( print_action_header(theAction); )

  if (theAction.numInts > 0)
    theAction.intArray = (int*)calloc(theAction.numInts, sizeof(int));

  if (theAction.numDoubles > 0)
    theAction.doubleArray = (double*)calloc(theAction.numDoubles, sizeof(int));

  rlRecvActionBody(theConnection, &theAction);
  return theAction;
}

Action agent_step(Reward theReward, Observation theObservation)
{
  send_msg(theConnection, kStep);
  
  rlSendReward(theConnection, theReward);
  RLNET_DEBUG( print_reward(theReward); )

  rlSendObservation(theConnection, theObservation);
  RLNET_DEBUG( print_observation_header(theObservation); )

  rlRecvAction(theConnection, &theAction);
  RLNET_DEBUG( print_action_header(theAction); )

  return theAction;
}

void agent_end(Reward theReward)
{
  send_msg(theConnection, kEnd);

  rlSendReward(theConnection, theReward);
  RLNET_DEBUG( print_reward(theReward); )

  free(theAction.intArray);
  free(theAction.doubleArray);

  theAction.intArray = 0;
  theAction.doubleArray = 0;
}

void agent_cleanup()
{
  send_msg(theConnection, kCleanup);
  rlClose(theConnection);
}
