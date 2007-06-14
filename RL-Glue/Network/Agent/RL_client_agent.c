#include <assert.h> /* assert  */
#include <stdlib.h> /* calloc */
#include <stdio.h>  /* fprintf */
#include <unistd.h> /* sleep   */
#include <string.h> /* strlen */

#include <RL_common.h>
#include <Network/RL_netlib.h>

/* Provide forward declaration of agent interface */
extern void   agent_init(Task_specification task_spec);
extern Action agent_start(Observation o);
extern Action agent_step(Reward r, Observation o);
extern void   agent_end(Reward r);
extern void   agent_cleanup();
extern void   agent_freeze();
extern char*  agent_message(const char* inMessage);

static const char* kUnknownMessage = "Unknown Message: %d\n";

static char* theTaskSpec = 0;
static Observation theObservation = {0};
static int isObservationAllocated = 0;

static void onAgentInit(rlSocket theConnection) {
  int theTaskSpecLength = 0;

  rlRecvData(theConnection, &theTaskSpecLength, sizeof(int));
  if (theTaskSpecLength > 0) {
    theTaskSpec = (char*)calloc(theTaskSpecLength, sizeof(char));
    rlRecvData(theConnection, theTaskSpec, sizeof(char) * theTaskSpecLength);
  }

  agent_init(theTaskSpec);
}

static void onAgentStart(rlSocket theConnection) {
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

static void onAgentStep(rlSocket theConnection) {
  Reward theReward = 0;
  Action theAction = {0};

  rlRecvData(theConnection, &theReward, sizeof(Reward));
  rlRecvADTHeader(theConnection, (RL_abstract_type*)&theObservation);
  rlRecvADTBody(theConnection, (RL_abstract_type*)&theObservation);

  theAction = agent_step(theReward, theObservation);
  rlSendADT(theConnection, (RL_abstract_type*)&theAction);
}

static void onAgentEnd(rlSocket theConnection) {
  Reward theReward = 0;

  rlRecvData(theConnection, &theReward, sizeof(Reward));
  agent_end(theReward);
}

static void onAgentCleanup(rlSocket theConnection) {
  agent_cleanup();

  rlFreeADT(&theObservation);
  isObservationAllocated = 0;

  free(theTaskSpec);
  theTaskSpec = 0;
}

static void onAgentFreeze(rlSocket theConnection) {
  agent_freeze();
}

static void onAgentMessage(rlSocket theConnection) {
  int theInMessageLength = 0;
  int theOutMessageLength = 0;
  char* theInMessage = NULL;
  char* theOutMessage = NULL;

  rlRecvData(theConnection, &theInMessageLength, sizeof(int));
  if (theInMessageLength > 0) {
    theInMessage = (char*)calloc(theInMessageLength, sizeof(char));
    rlRecvData(theConnection, theInMessage, sizeof(char) * theInMessageLength);
  }
  theOutMessage = agent_message(theInMessage);

  if (theOutMessage != NULL) {
   theOutMessageLength = strlen(theOutMessage)+1;
  }

  rlSendData(theConnection, &theOutMessageLength, sizeof(int));
  if (theOutMessageLength > 0) {
	rlSendData(theConnection, theOutMessage, sizeof(char)*theOutMessageLength);
  } 
}

static void runAgentEventLoop(rlSocket theConnection) {
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

    case kAgentFreeze:
      onAgentFreeze(theConnection);
      break;

    case kAgentMessage:
      onAgentMessage(theConnection);
	  break;
    
    default:
      fprintf(stderr, kUnknownMessage, agentState);
      break;
    };
  } while (agentState != kAgentCleanup);
}

int main(int argc, char** argv) {
  const int theConnectionType = kAgentConnection;
  rlSocket theConnection = 0;
  int arg = 0;
  int isDaemon = 0;

  for (arg = 0; arg < argc; ++arg) {
    if (strcmp(argv[arg], "--stayalive") == 0) {
      isDaemon = 1;
    }
  }

  do {
    theConnection = rlWaitForConnection(kLocalHost, kDefaultPort, kRetryTimeout);
    /* we need to tell RL-Glue what type of object is connecting */
    rlSendData(theConnection, &theConnectionType, sizeof(int)); 
    runAgentEventLoop(theConnection);
    rlClose(theConnection);
  } while(isDaemon);

  return 0;
}
