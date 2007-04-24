#include <RLcommon.h>
#include <RLnet/RLnet.h>

#ifdef NETWORK_DEBUG
#define RLNET_DEBUG(x) x
#else
#define RLNET_DEBUG(x)
#endif

const char* kRLInit = "init";
const char* kRLStart = "start";
const char* kRLStep = "step";
const char* kRLReturn = "return";
const char* kRLAverageReward = "aver";
const char* kRLAverageNumSteps = "avens";
const char* kRLNumSteps = "numstep";
const char* kRLNumEpisodes = "numeps";
const char* kRLEpisode = "episode";
const char* kRLSetState = "sets";
const char* kRLSetRandomSeed = "setrs";
const char* kRLGetState = "gets";
const char* kRLGetRandomSeed = "getrs";
const char* kRLCleanup = "cleanup";

void on_RL_init(rlSocket theConnection)
{
  RL_init();
}

void on_RL_start(rlSocket theConnection)
{
  RL_start();
}

void on_RL_step(rlSocket theConnection)
{
  RL_step();
}

void on_RL_return(rlSocket theConnection)
{
  RL_return();
}

void on_RL_average_reward(rlSocket theConnection)
{
  RL_average_reward();
}

void on_RL_average_num_steps(rlSocket theConnection)
{
  RL_average_num_steps();
}

void on_RL_num_episodes(rlSocket theConnection)
{
  RL_num_episodes();
}

void on_RL_num_steps(rlSocket theConnection)
{
  RL_num_steps();
}

void on_RL_num_episodes(rlSocket theConnection)
{
  RL_num_episodes();
}

void on_RL_episode(rlSocket theConnection)
{
  RL_episode();
}

void on_RL_set_state(rlSocket theConnection)
{
  RL_set_state();
}

void on_RL_set_random_seed(rlSocket theConnection)
{
  RL_set_random_seed();
}

void on_RL_get_state(rlSocket theConnection)
{
  RL_get_state();
}

void on_RL_get_random_seed(rlSocket theConnection)
{
  RL_get_random_seed();
}

void on_RL_cleanup(rlSocket theConnection)
{
  RL_cleanup();
}

void run_benchmark(rlSocket theConnection)
{
  char theMessage[8] = {0};

  do
  { 
    rlRecvData(theConnection, theMessage, 8);
    RLNET_DEBUG( fprintf(stderr, "GLUE RECV: %s\n", theMessage); )
              
    if (strncmp(theMessage, kRLInit, 8) == 0)
    {
      on_RL_init(theConnection);
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
    else if (strncmp(theMessage, kRLNumEpisodes, 8) == 0)
    {
      on_RL_num_episodes(theConnection);
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
  rlSocket theConnection;
  short thePort = 0;

  if (argc != 3) 
  {
    fprintf(stderr, kUsage);
    return 1;
  }

  sscanf(argv[2], "%hd", &thePort);

  theConnection = rlOpen(thePort);
  assert(rlIsValidSocket(theConnection));

  assert(rlConnect(theConnection, argv[1]) >= 0);
  run_benchmark(theConnection);
  assert(rlClose(theConnection) >= 0);

  return 0;
}
