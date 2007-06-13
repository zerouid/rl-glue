#include <stdlib.h> /* calloc */

#include <RL_common.h>
#include <Network/RL_netlib.h>

static rlSocket theEnvironmentConnection = 0;

static Task_specification theTaskSpec = 0;

static Observation theObservation       = {0};
static State_key theStateKey            = {0};
static Random_seed_key theRandomSeedKey = {0};

static int isObservationAllocated   = 0;
static int isStateKeyAllocated      = 0;
static int isRandomSeedKeyAllocated = 0;

void rlSetEnvironmentConnection(int theConnection) {
  if (theEnvironmentConnection) {
    rlClose(theEnvironmentConnection);
  }

  theEnvironmentConnection = theConnection;
}

int rlIsEnvironmentConnected() {
  return theEnvironmentConnection != 0;
}

Task_specification env_init() {
  const int envState = kEnvInit;
  int theTaskSpecLength = 0;

  rlSendData(theEnvironmentConnection, &envState, sizeof(int));
  rlRecvData(theEnvironmentConnection, &theTaskSpecLength, sizeof(int));
  
  if (theTaskSpecLength > 0) {
    theTaskSpec = (char*)calloc(theTaskSpecLength, sizeof(char));
    rlRecvData(theEnvironmentConnection, theTaskSpec, theTaskSpecLength);
  }
  
  return theTaskSpec;
}

Observation env_start() {
  const int envState = kEnvStart;
  rlSendData(theEnvironmentConnection, &envState, sizeof(int));

  rlRecvADTHeader(theEnvironmentConnection, &theObservation);
  if (!isObservationAllocated) {
    rlAllocADT(&theObservation);
    isObservationAllocated = 1;
  }
  rlRecvADTBody(theEnvironmentConnection, &theObservation);
  return theObservation;
}

Reward_observation env_step(Action theAction) {
  const int envState = kEnvStep;
  Reward_observation ro = {0};
  rlSendData(theEnvironmentConnection, &envState, sizeof(int));
  rlSendADT(theEnvironmentConnection, &theAction);

  rlRecvData(theEnvironmentConnection, &ro.r, sizeof(Reward));
  rlRecvADTHeader(theEnvironmentConnection, &theObservation);
  rlRecvADTBody(theEnvironmentConnection, &theObservation);
  rlRecvData(theEnvironmentConnection, &ro.terminal, sizeof(int));

  ro.o = theObservation;
  return ro;
}

void env_cleanup() {
  const int envState = kEnvCleanup;
  rlSendData(theEnvironmentConnection, &envState, sizeof(int));
  rlClose(theEnvironmentConnection);
  theEnvironmentConnection = 0;

  rlFreeADT(&theObservation);
  rlFreeADT(&theStateKey);
  rlFreeADT(&theRandomSeedKey);

  isObservationAllocated   = 0;
  isStateKeyAllocated      = 0;
  isRandomSeedKeyAllocated = 0;
}

void env_set_state(State_key theStateKey) {
  const int envState = kEnvSetState;
  rlSendData(theEnvironmentConnection, &envState, sizeof(int));
  rlSendADT(theEnvironmentConnection, &theStateKey);
}

void env_set_random_seed(Random_seed_key theRandomSeedKey) {
  const int envState = kEnvSetRandomSeed;
  rlSendData(theEnvironmentConnection, &envState, sizeof(int));
  rlSendADT(theEnvironmentConnection, &theRandomSeedKey);
}

State_key env_get_state() {
  const int envState = kEnvGetState;
  rlSendData(theEnvironmentConnection, &envState, sizeof(int));

  rlRecvADTHeader(theEnvironmentConnection, &theStateKey);
  if (!isStateKeyAllocated) {
    rlAllocADT(&theStateKey);
    isStateKeyAllocated = 1;
  }
  rlRecvADTBody(theEnvironmentConnection, &theStateKey);

  return theStateKey;
}

Random_seed_key env_get_random_seed() {
  const int envState = kEnvGetRandomSeed;
  rlSendData(theEnvironmentConnection, &envState, sizeof(int));

  rlRecvADTHeader(theEnvironmentConnection, &theRandomSeedKey);
  if (!isRandomSeedKeyAllocated) {
    rlAllocADT(&theRandomSeedKey);
    isRandomSeedKeyAllocated = 1;
  }
  rlRecvADTBody(theEnvironmentConnection, &theRandomSeedKey);

  return theRandomSeedKey;
}
