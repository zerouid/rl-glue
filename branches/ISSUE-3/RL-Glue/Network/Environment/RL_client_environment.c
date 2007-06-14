#include <assert.h> /* assert  */
#include <unistd.h> /* sleep   */
#include <string.h> /* strlen  */
#include <stdio.h>  /* fprintf */
#include <stdlib.h> /* calloc */

#include <RL_common.h>
#include <Network/RL_netlib.h>

/* Provide forward declaration of environment interface */
extern Task_specification env_init();
extern Observation env_start();
extern Reward_observation env_step(Action a);
extern void env_cleanup();
extern void env_set_state(State_key sk);
extern void env_set_random_seed(Random_seed_key rsk);
extern State_key env_get_state();
extern Random_seed_key env_get_random_seed();
extern char* env_message(const char* inMessage);

static const char* kUnknownMessage = "Unknown Message: %s\n";

static Action theAction                 = {0};
static State_key theStateKey            = {0};
static Random_seed_key theRandomSeedKey = {0};

static int isActionAllocated        = 0;
static int isStateKeyAllocated      = 0;
static int isRandomSeedKeyAllocated = 0;

static void onEnvInit(rlSocket theConnection) {
  Task_specification theTaskSpec = 0;
  int theTaskSpecLength = 0;

  theTaskSpec = env_init();
  
  if (theTaskSpec != NULL) {
    theTaskSpecLength = strlen(theTaskSpec)+1;
  }

  rlSendData(theConnection, &theTaskSpecLength, sizeof(int));

  if (theTaskSpecLength > 0) {
    rlSendData(theConnection, theTaskSpec, sizeof(char) * theTaskSpecLength);
  }
}

static void onEnvStart(rlSocket theConnection) {
  Observation theObservation = env_start();
  rlSendADT(theConnection, &theObservation);
}

static void onEnvStep(rlSocket theConnection) {
  Reward_observation ro = {0};

  rlRecvADTHeader(theConnection, &theAction);
  if (!isActionAllocated) {
    rlAllocADT(&theAction);
    isActionAllocated = 1;
  }
  rlRecvADTBody(theConnection, &theAction);

  ro = env_step(theAction);

  rlSendData(theConnection, &ro.r, sizeof(Reward));
  rlSendADT(theConnection, &ro.o);
  rlSendData(theConnection, &ro.terminal, sizeof(int));
}

static void onEnvCleanup(rlSocket theConnection) {
  env_cleanup();
  
  if (isActionAllocated) {
    rlFreeADT(&theAction);
  }

  if (isStateKeyAllocated) {
    rlFreeADT(&theStateKey);
  }

  if (isRandomSeedKeyAllocated) {
    rlFreeADT(&theRandomSeedKey);
  }

  isActionAllocated        = 0;
  isStateKeyAllocated      = 0;
  isRandomSeedKeyAllocated = 0;
}

static void onEnvSetState(rlSocket theConnection) {
  rlRecvADTHeader(theConnection, &theStateKey);
  if (!isStateKeyAllocated) {
    rlAllocADT(&theStateKey);
    isStateKeyAllocated = 1;
  }
  rlRecvADTBody(theConnection, &theStateKey);

  env_set_state(theStateKey);
}

static void onEnvSetRandomSeed(rlSocket theConnection) {
  rlRecvADTHeader(theConnection, &theRandomSeedKey);
  if (!isRandomSeedKeyAllocated) {
    rlAllocADT(&theRandomSeedKey);
    isRandomSeedKeyAllocated = 1;
  }
  rlRecvADTBody(theConnection, &theRandomSeedKey);
  
  env_set_random_seed(theRandomSeedKey);
}

static void onEnvGetState(rlSocket theConnection) {
  State_key key = env_get_state(); 
  rlSendADT(theConnection, &key);
}

static void onEnvGetRandomSeed(rlSocket theConnection) {
  Random_seed_key key = env_get_random_seed();
  rlSendADT(theConnection, &key);
}

static void onEnvMessage(rlSocket theConnection) {
  int theInMessageLength = 0;
  int theOutMessageLength = 0;
  char* theInMessage = NULL;
  char* theOutMessage = NULL;

  rlRecvData(theConnection, &theInMessageLength, sizeof(int));
  if (theInMessageLength > 0) {
    theInMessage = (char*)calloc(theInMessageLength, sizeof(char));
    rlRecvData(theConnection, theInMessage, sizeof(char) * theInMessageLength);
  }
  theOutMessage = env_message(theInMessage);

  if (theOutMessage != NULL)
   theOutMessageLength = strlen(theOutMessage)+1;

  rlSendData(theConnection, &theOutMessageLength, sizeof(int));
  if (theOutMessageLength > 0) {
	rlSendData(theConnection, theOutMessage, sizeof(char)*theOutMessageLength);
  }
}

static void runEnvironmentEventLoop(rlSocket theConnection) {
  int envState = 0;

  do { 
    rlRecvData(theConnection, &envState, sizeof(int));

    switch(envState) {
    case kEnvInit:
      onEnvInit(theConnection);
      break;

    case kEnvStart:
      onEnvStart(theConnection);
      break;

    case kEnvStep:
      onEnvStep(theConnection);
      break;

    case kEnvCleanup:
      onEnvCleanup(theConnection);
      break;

    case kEnvSetState:
      onEnvSetState(theConnection);
      break;

    case kEnvSetRandomSeed:
      onEnvSetRandomSeed(theConnection);
      break;

    case kEnvGetState:
      onEnvGetState(theConnection);
      break;

    case kEnvGetRandomSeed:
      onEnvGetRandomSeed(theConnection);
      break;

    case kEnvMessage:
      onEnvMessage(theConnection);
      break;

    default:
      fprintf(stderr, kUnknownMessage, envState);
      break;
    };
  } while (envState != kEnvCleanup);
}

int main(int argc, char** argv) {
  const int theConnectionType = kEnvironmentConnection;
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
    runEnvironmentEventLoop(theConnection);
    rlClose(theConnection);
  } while (isDaemon);

  return 0;
}
