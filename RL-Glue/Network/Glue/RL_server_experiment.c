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
              
    if (strncmp(theMessage, kRLInit, 8) == 0)
    {
      on_RL_init(argc, argv, theConnection);
    }
    else if (strncmp(theMessage, kRLStart, 8) == 0)
    {
      on_RL_start(theConnection);
    }
    else if (strncmp(theMessage, kRLStep, 8) == 0)
    {
      on_RL_step(theConnection);
    }
    else if ( strncmp(theMessage, kRLReturn, 8) == 0)
    {
      on_RL_return(theConnection);
    }
    else if (strncmp(theMessage, kRLAverageReward, 8) == 0)
    {
      on_RL_average_reward(theConnection);
    }
    else if (strncmp(theMessage, kRLAverageNumSteps, 8) == 0)
    {
      on_RL_average_num_steps(theConnection);
    }
    else if (strncmp(theMessage, kRLNumSteps, 8) == 0)
    {
      on_RL_num_steps(theConnection);
    }
    else if (strncmp(theMessage, kRLNumEpisodes, 8) == 0)
    {
      on_RL_num_episodes(theConnection);
    }
    else if (strncmp(theMessage, kRLEpisode, 8) == 0)
    {
      on_RL_episode(theConnection);
    }
    else if (strncmp(theMessage, kRLSetState, 8) == 0)
    {
      on_RL_set_state(theConnection);
    }
    else if (strncmp(theMessage, kRLSetRandomSeed, 8) == 0)
    {
      on_RL_set_random_seed(theConnection);
    }
    else if (strncmp(theMessage, kRLGetState, 8) == 0)
    {
      on_RL_get_state(theConnection);
    }
    else if (strncmp(theMessage, kRLGetRandomSeed, 8) == 0)
    {
      on_RL_get_random_seed(theConnection);
    }    
    else if (strncmp(theMessage, kRLCleanup, 8) == 0)
    {
      on_RL_cleanup(theConnection);
    }
    else
    {
      fprintf(stderr, kUnknownMessage, theMessage);
      break;
    }
  } while (strncmp(theMessage, kRLCleanup, 8) != 0);
}

int main(int argc, char** argv)
{
  rlSocket theServer;
  rlSocket theConnection;

  int isValidSocket = 0;
  int isListening = 0;

  short thePort = kDefaultPort;
  parse_command_line(argc, argv, &thePort);

  while(1)
  {
    theServer = rlOpen(thePort);
    isValidSocket = rlIsValidSocket(theServer);
    assert(isValidSocket);

    isListening = rlListen(theServer);
    assert(isListening >= 0);

    theConnection = rlAcceptConnection(theServer);
    isValidSocket = rlIsValidSocket(theConnection);
    assert(isValidSocket);

    rlClose(theServer);
    
    run_experiment(argc, argv, theConnection);
    rlClose(theConnection);
  }

  return 0;
}
