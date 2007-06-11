#include <stdio.h> /* fprintf */

#include <RL_common.h>
#include <Network/RL_netlib.h>

const char* kUnknownMessage = "Unknown Message: %s\n";

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

void onRLInit(rlSocket theConnection) {
	RL_init();
}

void onRLStart(rlSocket theConnection) {
	RL_start();
}

void onRLStep(rlSocket theConnection) {
	RL_step();
}

void onRLReturn(rlSocket theConnection) {
	RL_return();
}

void onRLNumSteps(rlSocket theConnection) {
	RL_num_steps();
}

void onRLNumEpisodes(rlSocket theConnection) {
	RL_num_episodes();
}

void onRLEpisode(rlSocket theConnection) {
	RL_episode();
}

void onRLSetState(rlSocket theConnection) {
	RL_set_state();
}

void onRLSetRandomSeed(rlSocket theConnection) {
	RL_set_random_seed();
}

void onRLGetState(rlSocket theConnection) {
	RL_get_state();
}

void onRLGetRandomSeed(rlSocket theConnection) {
	RL_get_random_seed();
}

void onRLCleanup(rlSocket theConnection) {
	RL_cleanup();
}

void runGlueEventLoop(rlSocket theConnection) {
  int glueState = 0;
  do { 
    rlRecvData(theConnection, &glueState, sizeof(int));

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

int main(int argc, char** argv)
{
  while(1) {
	rlConnectSystems();
    runGlueEventLoop();
  }

  return 0;
}
