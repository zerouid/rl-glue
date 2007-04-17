#include "RLnet_agent.h"

#include "RLnet.h"

char send_buffer[4] = {0};
char recv_buffer[8] = {0};

unsigned int theTaskSpecLength = 0;
char* theTaskSpecBuffer = 0;

Action action;
Observation observation;
Reward reward;

//task_spec_struct theTaskSpec;

void on_agent_init(rlSocket theSocket)
{
  rlRecvData(theSocket, theTaskSpecLength, sizeof(unsigned int));
  rlRecvData(theSocket, theTaskSpecBuffer, 2048);
 
  agent_init(theTaskSpecBuffer);

  // allocate observation
  // allocate action
  // allocate reward
}

void on_agent_start(rlSocket theSocket)
{
  // recv observation, spec.obs_dim
  action = agent_start(observation);
  // send action, spec.action_dim
}

void on_agent_step(rlSocket theSocket)
{
  // recv the reward
  // recv the observation, spec.obs_dim
  action = agent_step(reward, observation);
  // send action spec.action_dim
}

void on_agent_end(rlSocket theSocket)
{
  // recv reward
  agent_end(reward);
}

void on_agent_cleanup(rlSocket theSocket)
{
  agent_cleanup();

  //deallocate( g_observation );
  //deallocate( g_action );
  //deallocate( g_reward );
}

void on_termination(rlSocket theSocket)
{
}

void run_agent(rlSocket theSocket)
{
  char theMessage[8] = {0};

  do
  { 
    rlRecvData(theSocket, theMessage, 8);
	      
    if (strncmp(theMessage, "init", 8) == 0)
    {
      on_agent_init(theSocket);
    }
    else if (strncmp(theMessage, "start", 8) == 0)
    {
      on_agent_start(theSocket);
    }
    else if (strncmp(theMessage, "step", 8) == 0)
    {
      on_agent_step(theSocket);
    }
    else if ( strncmp(theMessage, "end", 8) == 0)
    {
      on_agent_end(theSocket);
    }
    else if (strncmp(theMessage, "cleanup", 8) == 0)
    {
      on_agent_cleanup(theSocket);
    }
    else if (strncmp(theMessage, "term", 8) == 0)
    {
      on_termination(theSocket);
    }
    else
    {
      fprintf(stderr, "Unknown Message: %s\n", theMessage);
      break;
    }
  } while (strncmp(theMessage, "term", 8) != 0);
}

int main(int argc, char** argv)
{
  rlSocket theSocket;
  
  theSocket = rlOpen(4096);
  assert(rlIsValidSocket(theSocket));

  assert(rlConnect(theSocket, argv[1]) >= 0);
  run_agent(theSocket);
  assert(rlClose(theSocket) >= 0);

  return 0;
}
