#include <stdio.h>
#include <string.h>
#include <assert.h>

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

const char* kUnknownMessage = "Unknown Message: %s\n";
const char* kUsage = "Usage: Agent <ip-address> <port>\n";

extern void RL_init();
extern Observation_action RL_start();
extern Reward_observation_action_terminal RL_step();
extern Reward RL_return();
extern Reward RL_average_reward();
extern double RL_average_num_steps();
extern int RL_num_steps();
extern int RL_num_episodes();
/* extern void RL_episode(); */
extern void RL_episode(int num_steps);
extern void RL_set_state(State_key sk);
extern void RL_set_random_seed(Random_seed_key rsk);
extern State_key RL_get_state();
extern Random_seed_key RL_get_random_seed();
extern void RL_cleanup();

void on_RL_init(rlSocket theConnection)
{
  RL_init();
}

void on_RL_start(rlSocket theConnection)
{
  Observation_action theObservationAction = RL_start();

  rlSendObservationHeader(theConnection, theObservationAction.o);
  rlSendActionHeader(theConnection, theObservationAction.a);
}

void on_RL_step(rlSocket theConnection)
{
  Reward_observation_action_terminal theRewardObservationActionTerminal = RL_step();

  rlSendObservationHeader(theConnection, theRewardObservationActionTerminal.o);
  rlSendActionHeader(theConnection, theRewardObservationActionTerminal.a);

  rlSendReward(theConnection, theRewardObservationActionTerminal.r);
  rlSendObservationBody(theConnection, theRewardObservationActionTerminal.o);
  rlSendActionBody(theConnection, theRewardObservationActionTerminal.a);
  rlSendTerminal(theConnection, theRewardObservationActionTerminal.terminal);
}

void on_RL_return(rlSocket theConnection)
{
  Reward theReturn = RL_return();
  rlSendReward(theConnection, theReturn);
}

void on_RL_average_reward(rlSocket theConnection)
{
  Reward theAverageReward = RL_average_reward();
  rlSendReward(theConnection, theAverageReward);
}

void on_RL_average_num_steps(rlSocket theConnection)
{
  double theAverageNumberOfSteps = RL_average_num_steps();
  rlSendData(theConnection, &theAverageNumberOfSteps, sizeof(double));
}

void on_RL_num_steps(rlSocket theConnection)
{
  int theNumberOfSteps = RL_num_steps();
  rlSendData(theConnection, &theNumberOfSteps, sizeof(int));
}

void on_RL_num_episodes(rlSocket theConnection)
{
  int theNumberOfEpisodes = RL_num_episodes();
  rlSendData(theConnection, &theNumberOfEpisodes, sizeof(int));
}

void on_RL_episode(rlSocket theConnection)
{
  int numSteps = 0;
  rlRecvData(theConnection, &numSteps, sizeof(int));
  RL_episode(numSteps);
}

void on_RL_set_state(rlSocket theConnection)
{
  State_key theKey;
  rlRecvData(theConnection, &theKey, sizeof(State_key));
  RL_set_state(theKey);
}

void on_RL_set_random_seed(rlSocket theConnection)
{
  Random_seed_key theKey;
  rlRecvData(theConnection, &theKey, sizeof(Random_seed_key));
  RL_set_random_seed(theKey);
}

void on_RL_get_state(rlSocket theConnection)
{
  State_key theKey = RL_get_state();
  rlSendData(theConnection, &theKey, sizeof(State_key));
}

void on_RL_get_random_seed(rlSocket theConnection)
{
  Random_seed_key theKey = RL_get_random_seed();
  rlSendData(theConnection, &theKey, sizeof(Random_seed_key));
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
