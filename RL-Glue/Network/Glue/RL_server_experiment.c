#include <stdio.h> /* fprintf */

#include <assert.h> /* assert */
#include <RL_common.h>
#include <Network/RL_netlib.h>

const char* kUnknownMessage = "Unknown Message: %s\n";

extern int rlConnectSystems();

extern void RL_init();
extern Observation_action RL_start();
extern Reward_observation_action_terminal RL_step();
extern Reward RL_return();
extern int RL_num_steps();
extern int RL_num_episodes();
extern void RL_episode(int num_steps);
extern void RL_set_state(State_key sk);
extern void RL_set_random_seed(Random_seed_key rsk);
extern State_key RL_get_state();
extern Random_seed_key RL_get_random_seed();
extern void RL_cleanup();

State_key theStateKey = {0};
Random_seed_key theRandomSeedKey = {0};
int isStateKeyAllocated = 0;
int isRandomSeedKeyAllocated = 0;

void onRLInit(rlSocket theConnection) {
  RL_init();
}

void onRLStart(rlSocket theConnection) {
  Observation_action obsAct = RL_start();

  rlSendADT(theConnection, &obsAct.o);
  rlSendADT(theConnection, &obsAct.a);
}

void onRLStep(rlSocket theConnection) {
  Reward_observation_action_terminal roat = RL_step();
  
  rlSendData(theConnection, &roat.r, sizeof(Reward));
  rlSendADT(theConnection, &roat.o);
  rlSendADT(theConnection, &roat.a);
  rlSendData(theConnection, &roat.terminal, sizeof(int));
}

void onRLReturn(rlSocket theConnection) {
  Reward theReward = RL_return();
  rlSendData(theConnection, &theReward, sizeof(int));
}

void onRLNumSteps(rlSocket theConnection) {
  int numSteps = RL_num_steps();
  rlSendData(theConnection, &numSteps, sizeof(int));
}

void onRLNumEpisodes(rlSocket theConnection) {
  int numEpisodes = RL_num_episodes();
  rlSendData(theConnection, &numEpisodes, sizeof(int));
}

void onRLEpisode(rlSocket theConnection) {
  int numEpisodes = 0;
  
  rlRecvData(theConnection, &numEpisodes, sizeof(int));
  RL_episode(numEpisodes);
}

void onRLSetState(rlSocket theConnection) {
  rlRecvADTHeader(theConnection, &theStateKey);
  if (!isStateKeyAllocated) {
    rlAllocADT(&theStateKey);
    isStateKeyAllocated = 1;
  }
  rlRecvADTBody(theConnection, &theStateKey);
  
  RL_set_state(theStateKey);
}

void onRLSetRandomSeed(rlSocket theConnection) {	
  rlRecvADTHeader(theConnection, &theRandomSeedKey);
  if (!isRandomSeedKeyAllocated) {
    rlAllocADT(&theRandomSeedKey);
    isRandomSeedKeyAllocated = 1;
  }
  rlRecvADTBody(theConnection, &theRandomSeedKey);
  
  RL_set_random_seed(theRandomSeedKey);
}

void onRLGetState(rlSocket theConnection) {
  State_key theStateKey = RL_get_state();
  rlSendADT(theConnection, &theStateKey);
}

void onRLGetRandomSeed(rlSocket theConnection) {
  Random_seed_key theRandomSeedKey = RL_get_random_seed();
  rlSendADT(theConnection, &theRandomSeedKey);
}

void onRLCleanup(rlSocket theConnection) {
  RL_cleanup();
  
  rlFreeADT(&theStateKey);
  rlFreeADT(&theRandomSeedKey);
  
  isStateKeyAllocated      = 0;
  isRandomSeedKeyAllocated = 0;
}

void runGlueEventLoop(rlSocket theConnection) {
  int glueState = 0;

  do { 
    rlRecvData(theConnection, &glueState, sizeof(int));
    fprintf(stderr, "glueState = %d\n", glueState);

    switch(glueState) {
    case kRLInit:
      onRLInit(theConnection);
      break;
      
    case kRLStart:
      onRLStart(theConnection);
      break;
      
    case kRLStep:
      onRLStep(theConnection);
      break;
      
    case kRLReturn:
      onRLReturn(theConnection);
      break;
      
    case kRLCleanup:
      onRLCleanup(theConnection);
      break;
      
    case kRLNumSteps:
      onRLNumSteps(theConnection);
      break;
      
    case kRLNumEpisodes:
      onRLNumEpisodes(theConnection);
      break;
      
    case kRLEpisode:
      onRLEpisode(theConnection);
      break;
      
    case kRLSetState:
      onRLSetState(theConnection);
      break;
      
    case kRLSetRandomSeed:
      onRLSetRandomSeed(theConnection);
      break;
      
    case kRLGetState:
      onRLGetState(theConnection);
      break;
      
    case kRLGetRandomSeed:
      onRLGetRandomSeed(theConnection);
      break;
      
    default:
      fprintf(stderr, kUnknownMessage, glueState);
      break;
    };
  } while (glueState != kRLCleanup);
}

int main(int argc, char** argv) {
  rlSocket theConnection = 0;
  while(1) {
    theConnection = rlConnectSystems();
    assert(rlIsValidSocket(theConnection));
    runGlueEventLoop(theConnection);
    rlClose(theConnection);
  }
  return 0;
}
