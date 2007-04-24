#include <assert.h>

#include <RLcommon.h>
#include <RLnet/RLnet.h>

static rlSocket theGlueConnection;

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
const char* kRLCleanup = "cleanup";

static void send_msg(rlSocket theSocket, const char* theMessage)
{
  char send_buffer[8] = {0};
  strncpy(send_buffer, theMessage, 8);
  rlSendData(theSocket, send_buffer, 8);

  RLNET_DEBUG( fprintf(stderr, "GLUE SENT: %s\n", send_buffer); )
}

void RL_init()
{
  int theTaskSpecLength = 0;
  rlSocket theServer = rlOpen(4095);
  assert(rlIsValidSocket(theServer));
  assert(rlListen(theServer) >= 0);
  theGlueConnection = rlAcceptConnection(theServer);
  rlClose(theServer);

  assert(rlIsValidSocket(theAgentConnection));
 
  send_msg(theGlueConnection, kRLInit);
}

Observation_action RL_start()
{
  send_msg(theGlueConnection, kRLStart);
}

Reward_observation_action_terminal RL_step()
{
  send_msg(theGlueConnection, kRLStep);
}

Reward RL_return()
{
  send_msg(theGlueConnection, kRLReturn);
}

Reward RL_average_reward()
{
  send_msg(theGlueConnection, kRLAverageReward);
}

double RL_average_num_steps()
{
  send_msg(theGlueConnection, kRLAverageNumSteps);
}

int RL_num_steps()
{
  send_msg(theGlueConnection, kRLNumSteps);
}

int RL_num_episodes()
{
  send_msg(theGlueConnection, kRLNumEpisodes);
}

/* void RL_episode(); */
void RL_episode(int num_steps)
{
  send_msg(theGlueConnection, kRLEpisode);
}

void RL_set_state(State_key sk)
{
  send_msg(theGlueConnection, kRLSetState);
}

void RL_set_random_seed(Random_seed_key rsk)
{
  send_msg(theGlueConnection, kRLSetRandomSeed);
}

State_key RL_get_state()
{
  send_msg(theGlueConnection, kRLGetState);
}

Random_seed_key RL_get_random_seed()
{
  send_msg(theGlueConnection, kRLGetRandomSeed);
}

void RL_cleanup()
{
  send_msg(theGlueConnection, kRLCleanup);
}
