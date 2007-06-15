#include <assert.h> /* assert  */
#include <unistd.h> /* sleep   */
#include <string.h> /* strlen, strncmp, strncpy  */ /* I'm sorry about using strlen. */
#include <stdio.h>  /* fprintf */
#include <stdlib.h> /* calloc, exit */
#include <ctype.h> /* isdigit */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

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
static rlBuffer theBuffer               = {0};
static char* theInMessage = 0;
static int theInMessageCapacity = 0;

static void onEnvInit(rlSocket theConnection) {
  Task_specification theTaskSpec = 0;
  int theTaskSpecLength = 0;
  int offset = 0;

  theTaskSpec = env_init();
  if (theTaskSpec != NULL) {
    theTaskSpecLength = strlen(theTaskSpec)+1;
  }

  rlBufferClear(&theBuffer);
  offset = rlBufferWrite(&theBuffer, offset, &theTaskSpecLength, 1, sizeof(int));
  if (theTaskSpecLength > 0) {
    offset = rlBufferWrite(&theBuffer, offset, theTaskSpec, theTaskSpecLength, sizeof(char));
  }

  rlSendBufferData(theConnection, &theBuffer);
}

static void onEnvStart(rlSocket theConnection) {
  Observation theObservation = env_start();
  
  rlCopyADTToBuffer(&theObservation, &theBuffer);
  rlSendBufferData(theConnection, &theBuffer);
}

static void onEnvStep(rlSocket theConnection) {
  Reward_observation ro = {0};
  int offset = 0;

  rlBufferClear(&theBuffer);
  rlRecvBufferData(theConnection, &theBuffer);
  rlCopyBufferToADT(&theBuffer, &theAction);

  ro = env_step(theAction);

  rlBufferClear(&theBuffer);
  offset = rlBufferWrite(&theBuffer, offset, &ro.terminal, 1, sizeof(int));
  offset = rlBufferWrite(&theBuffer, offset, &ro.r, 1, sizeof(Reward));
  rlSendBufferData(theConnection, &theBuffer);

  rlBufferClear(&theBuffer);
  rlCopyADTToBuffer(&ro.o, &theBuffer);
  rlSendBufferData(theConnection, &theBuffer);
}

static void onEnvCleanup(rlSocket theConnection) {
  env_cleanup();

  free(theAction.intArray);
  free(theAction.doubleArray);
  free(theRandomSeedKey.intArray);
  free(theRandomSeedKey.doubleArray);
  free(theStateKey.intArray);
  free(theStateKey.doubleArray);
  free(theInMessage);

  theAction.intArray           = 0;
  theAction.doubleArray        = 0;
  theRandomSeedKey.intArray    = 0;
  theRandomSeedKey.doubleArray = 0;
  theStateKey.intArray         = 0;
  theStateKey.doubleArray      = 0;

  theAction.numInts = 0;
  theAction.numDoubles = 0;
  theRandomSeedKey.numInts = 0;
  theRandomSeedKey.numDoubles = 0;
  theStateKey.numInts = 0;
  theStateKey.numDoubles = 0;

  theInMessage = 0;
  theInMessageCapacity = 0;
}

static void onEnvSetState(rlSocket theConnection) {
  rlBufferClear(&theBuffer);
  rlRecvBufferData(theConnection, &theBuffer);
  rlCopyBufferToADT(&theBuffer, &theStateKey);

  env_set_state(theStateKey);
}

static void onEnvSetRandomSeed(rlSocket theConnection) {
  rlBufferClear(&theBuffer);
  rlRecvBufferData(theConnection, &theBuffer);
  rlCopyBufferToADT(&theBuffer, &theRandomSeedKey);
  
  env_set_random_seed(theRandomSeedKey);
}

static void onEnvGetState(rlSocket theConnection) {
  State_key key = env_get_state();
 
  rlBufferClear(&theBuffer);
  rlCopyADTToBuffer(&key, &theBuffer);
  rlSendBufferData(theConnection, &theBuffer);
}

static void onEnvGetRandomSeed(rlSocket theConnection) {
  Random_seed_key key = env_get_random_seed();

  rlBufferClear(&theBuffer);
  rlCopyADTToBuffer(&key, &theBuffer);
  rlSendBufferData(theConnection, &theBuffer);
}

static void onEnvMessage(rlSocket theConnection) {
  int inMessageLength = 0;
  int outMessageLength = 0;
  char* inMessage = 0;
  char* outMessage = 0;
  int offset = 0;

  rlBufferClear(&theBuffer);
  rlRecvBufferData(theConnection, &theBuffer);

  offset = 0;
  offset = rlBufferRead(&theBuffer, offset, &inMessageLength, 1, sizeof(int));

  if (inMessageLength > theInMessageCapacity) {
    inMessage = (char*)calloc(inMessageLength, sizeof(char));
    free(theInMessage);

    theInMessage = inMessage;
    theInMessageCapacity = inMessageLength;
  }

  if (inMessageLength > 0) {
    offset = rlBufferRead(&theBuffer, offset, theInMessage, inMessageLength, sizeof(char));
  }

  outMessage = env_message(theInMessage);

  if (outMessage != NULL) {
   outMessageLength = strlen(outMessage)+1;
  }
  
  /* we want to start sending now, so we're going to reset the offset to 0 so we write the the beginning of the buffer */
  offset = 0;

  rlBufferClear(&theBuffer);
  offset = rlBufferWrite(&theBuffer, offset, &outMessageLength, 1, sizeof(int));
  
  if (outMessageLength > 0) {
    offset = rlBufferWrite(&theBuffer, offset, outMessage, outMessageLength, sizeof(char));
  }

  rlSendBufferData(theConnection, &theBuffer);
}

static void runEnvironmentEventLoop(rlSocket theConnection) {
  int envState = 0;

  do { 
    rlBufferClear(&theBuffer);
    rlRecvBufferData(theConnection, &theBuffer);
    rlBufferRead(&theBuffer, 0, &envState, 1, sizeof(int));

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
      exit(0);
      break;
    };
  } while (envState != kEnvCleanup);
}

int main(int argc, char** argv) {
  const int theConnectionType = kEnvironmentConnection;
  rlSocket theConnection = 0;

  int arg = 0;
  int isDaemon = 0;
  short port = kDefaultPort;
  char host[1024] = {0};
  struct hostent *host_ent;

  strncpy(host, kLocalHost, 1024);

  /* less gross than getlongopt */
  for (arg = 0; arg < argc; ++arg) {
    if (strncmp(argv[arg], "--stayalive", 12) == 0) {
      isDaemon = 1;
    }
    else if (sscanf(argv[arg], "--port = %hd", &port) != 0) {
    }
    else if (sscanf(argv[arg], "--host = %s", host) != 0) {
      if (isdigit(host[0])) {
	/* assume we got an ip address */
      }
      else if (isalpha(host[0])) {
	/* assume we got a host name */
	host_ent = gethostbyname(host);
	if (host_ent != 0) {
	  strncpy(host, inet_ntoa(*((struct in_addr*)host_ent->h_addr)), 1024);
	}
      }
    }
  }

  /* Allocate what should be plenty of space for the buffer - it will dynamically resize as needed */
  rlBufferCreate(&theBuffer, 4096);

  do {
    theConnection = rlWaitForConnection(kLocalHost, port, kRetryTimeout);
    /* we need to tell RL-Glue what type of object is connecting */

    rlBufferClear(&theBuffer);
    rlBufferWrite(&theBuffer, 0, &theConnectionType, 1, sizeof(int));
    rlSendBufferData(theConnection, &theBuffer);

    runEnvironmentEventLoop(theConnection);
    rlClose(theConnection);
  } while (isDaemon);

  rlBufferDestroy(&theBuffer);

  return 0;
}
