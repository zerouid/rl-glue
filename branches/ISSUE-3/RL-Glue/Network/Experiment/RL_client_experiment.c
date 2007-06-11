#include <RL_common.h>
#include <Network/RL_netlib.h>

static rlSocket theExperimentConnection = 0;

static Observation theObservation       = {0};
static Action theAction                 = {0};
static State_key theStateKey            = {0};
static Random_seed_key theRandomSeedKey = {0};

static int isObservationAllocated   = 0;
static int isActionAllocated        = 0;
static int isStateKeyAllocated      = 0;
static int isRandomSeedKeyAllocated = 0;

void RL_init() {
  const int theConnectionType = kExperimentConnection;
  const int experimentState = kRLInit;

  theExperimentConnection = rlWaitForConnection(kLocalHost, kDefaultPort, kRetryTimeout);
  rlSendData(theExperimentConnection, &theConnectionType, sizeof(int)); 
  rlSendData(theExperimentConnection, &experimentState, sizeof(int));
}

Observation_action RL_start() {
  const int experimentState = kRLStart;
  Observation_action oa = {0};

  rlSendData(theExperimentConnection, &experimentStart, sizeof(int));

  rlRecvADTHeader(theExperimentConnection, &theObservation);
  if (!isObservationAllocated) {
    rlAllocADT(&theObservation);
    isObservationAllocated = 1;
  }
  rlRecvADTBody(theExperimentConnection, &theObservation);

  rlRecvADTHeader(theExperimentConnection, &theAction);
  if (!isActionAllocated) {
    rlAllocADT(&theAction);
    isActionAllocated = 1;
  }
  rlRecvADTBody(theExperimentConnection, &theAction);

	oa.o = theObservation;
	oa.a = theAction;
	return oa;
}

Reward_observation_action_terminal RL_step() {
  const int experimentState = kRLStep;
  Reward_observation_action_terminal roat = {0};

  rlSendData(theExperimentConnection, &experimentState, sizeof(int));

  /* Recv Reward */
  rlRecvData(theExperimentConnection, &roat.r, sizeof(Reward));

  /* Recv Observation */
  rlRecvADTHeader(theExperimentConnection, &theObservation);
  rlRecvADTBody(theExperimentConnection, &theObservation);

  /* Recv Action */
  rlRecvADTHeader(theExperimentConnection, &theAction);
  rlRecvADTBody(theExperimentConnection, &theAction);

  /* Recv Terminal */
  rlRecvData(theExperimentConnection, &roat.terminal, sizeof(int));

  roat.o = theObservation;
  roat.a = theAction;

  return roat;
}

void RL_cleanup() {
  const int experimentState = kRLCleanup;

  rlSendData(theExperimentConnection, &experimentState, sizeof(int));

  rlClose(theExperimentConnection);
  theExperimentConnection = 0;

  if (isObservationAllocated) {
    rlFreeADT(&theObservation);
    isObservationAllocated = 0;
  }

  if (isActionAllocated) {
    rlFreeADT(&theAction);
    isActionAllocated = 0;
  }

  if (isStateKeyAllocated) {
    rlFreeADT(&theStateKey);
    isStateKeyAllocated = 0;
  }

  if (isRandomSeedKeyAllocated) {
    rlFreeADT(&theRandomSeedKey);
    isRandomSeedKeyAllocated = 0;
  }
}

Reward RL_return() {
  const int experimentState = kRLReturn;
  Reward theReward = 0;

  rlSendData(theExperimentConnection, &experimentState, sizeof(int));
  rlRecvData(theExperimentConnection, &theReward, sizeof(Reward));

  return theReward;
}

int RL_num_steps() {
  const int experimentState = kRLNumSteps;
  int numSteps = 0;

  rlSendData(theExperimentConnection, &experimentState, sizeof(int));
  rlRecvData(theExperimentConnection, &numSteps, sizeof(int));

  return numSteps;
}

int RL_num_episodes() {
  const int experimentState = kRLNumEpisodes;
  int numEpisodes = 0;

  rlSendData(theExperimentConnection, &experimentState, sizeof(int));
  rlRecvData(theExperimentConnection, &numEpisodes, sizeof(int));

  return numEpisodes;
}

void RL_episode(int numSteps) {
  const int experimentState = kRLEpisode;

  rlSendData(theExperimentConnection, &experimentState, sizeof(int));
  rlSendData(theExperimentConnection, &numSteps, sizeof(int));
}

void RL_set_state(State_key theStateKey) {
  const int experimentState = kRLSetState;

  rlSendData(theExperimentConnection, &experimentState, sizeof(int));
  rlSendADT(theExperimentConnection, &theStateKey);
}

void RL_set_random_seed(Random_seed_key theRandomSeedKey) {
  const int experimentState = kRLSetRandomSeed;

  rlSendData(theExperimentConnection, &experimentState, sizeof(int));
  rlSendADT(theExperimentConnection, &theRandomSeedKey);
}

State_key RL_get_state() {
  const int experimentState = kRLGetState;

  rlSendData(theExperimentConnection, &experimentState, sizeof(int));
  rlRecvADTHeader(theExperimentConnection, &theStateKey);
  if (!isStateKeyAllocated) {
    rlAllocADT(&theStateKey);
    isStateKeyAllocated = 1;
  }
  rlRecvADTBody(theExperimentConnection, &theStateKey);

  return theStateKey;
}

Random_seed_key RL_get_random_seed() {
  const int experimentState = kRLGetRandomSeed;

  rlSendData(theExperimentConnection, &experimentState, sizeof(int));
  rlRecvADTHeader(theExperimentConnection, &theRandomSeedKey);
  if (!isRandomSeedKeyAllocated) {
    rlAllocADT(&theRandomSeedKey);
    isRandomSeedKeyAllocated = 1;
  }
  rlRecvADTBody(theExperimentConnection, &theRandomSeedKey);

	return theRandomSeedKey
}
