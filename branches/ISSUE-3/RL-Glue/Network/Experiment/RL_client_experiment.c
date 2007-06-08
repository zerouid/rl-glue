#include <RL_common.h>
#include <Network/RL_netlib.h>

static rlSocket theExperimentConnection = 0;

static rlSocket waitForConnection(const char* address, const short port, const int retryTimeout) {
  rlSocket theConnection = 0;
  int isConnected = -1;

  while(isConnected == -1) {
    theConnection = rlOpen(port);
    assert(rlIsValidSocket(theConnection));
    isConnected = rlConnect(theConnection, address, port);
    if (isConnected == -1) { 
      rlClose(theConnection);
      sleep(retryTimeout);
    }
  }

  return theConnection;
}

void RL_init() {
  int theConnectionType = kExperimentConnection;
  theExperimentConnection = waitForConnection(kLocalHost, kDefaultPort, kRetryTimeout);
}

Observation_action RL_start() {
}

Reward_observation_action_terminal RL_step() {
}

Reward RL_return() {
}

int RL_num_steps() {
}

int RL_num_episodes() {
}

void RL_episode(int numSteps) {
}

void RL_set_state(State_key theStateKey) {
}

void RL_set_random_seed(Random_seed_key theRandomSeedKey) {
}

State_key RL_get_state() {
}

Random_seed_key RL_get_random_seed() {
}

void RL_cleanup() {
}
