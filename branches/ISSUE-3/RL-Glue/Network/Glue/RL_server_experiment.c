#include <RL_common.h>
#include <Network/RL_netlib.h>

const char* kUnknownMessage = "Unknown Message: %s\n";

extern void RL_init();
extern Observation_action RL_start();
extern Reward_observation_action_terminal RL_step();
extern Reward RL_return();
extern Reward RL_average_reward();
extern double RL_average_num_steps();
extern int RL_num_steps();
extern int RL_num_episodes();
extern void RL_episode(int num_steps);
extern void RL_set_state(State_key sk);
extern void RL_set_random_seed(Random_seed_key rsk);
extern State_key RL_get_state();
extern Random_seed_key RL_get_random_seed();
extern void RL_cleanup();

void onRLInit(rlSocket theConnection) {
}

void onRLStart(rlSocket theConnection) {
}

void onRLStep(rlSocket theConnection) {
}

void onRLReturn(rlSocket theConnection) {
}

void onRLNumSteps(rlSocket theConnection) {
}

void onRLNumEpisodes(rlSocket theConnection) {
}

void onRLEpisode(rlSocket theConnection) {
}

void onRLSetState(rlSocket theConnection) {
}

void onRLSetRandomSeed(rlSocket theConnection) {
}

void onRLGetState(rlSocket theConnection) {
}

void onRLGetRandomSeed(rlSocket theConnection) {
}

void onRLCleanup(rlSocket theConnection) {
}

void runGlueEventLoop(rlSocket theConnection) {
  int glueState = 0;
  do { 
    rlRecvData(theConnection, glueState, sizeof(int));

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
    rlRunGlueEventLoop();
  }

  return 0;
}
