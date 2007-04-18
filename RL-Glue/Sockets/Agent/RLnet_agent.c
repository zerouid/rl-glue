#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <RLcommon.h>
#include <RLnet/RLnet.h>

/* Declare the task spec and length */
unsigned int theTaskSpecLength;
char* theTaskSpecBuffer;

/* Declare observation action and reward */
Action action;
Observation observation;
Reward reward;

/* Provide forward declaration of agent interface */
extern void agent_init(Task_specification task_spec);
extern Action agent_start(Observation o);
extern Action agent_step(Reward r, Observation o);
extern void agent_end(Reward r);
extern void agent_cleanup();

void on_agent_init(rlSocket theSocket)
{
  theTaskSpecLength = 0;
  rlRecvData(theSocket, &theTaskSpecLength, sizeof(unsigned int));

  theTaskSpecBuffer = (char*)malloc(theTaskSpecLength);
  rlRecvData(theSocket, theTaskSpecBuffer, theTaskSpecLength);
 
  agent_init(theTaskSpecBuffer);

  /*
    allocate observation
    allocate action
  */
}

void on_agent_start(rlSocket theSocket)
{
  /* recv observation, spec.obs_dim */
  action = agent_start(observation);
  /* send action, spec.action_dim */
}

void on_agent_step(rlSocket theSocket)
{
  /* 
     recv the reward
     recv the observation, spec.obs_dim
  */
  action = agent_step(reward, observation);
  /*
    send action spec.action_dim
  */
}

void on_agent_end(rlSocket theSocket)
{
  /* recv reward */
  agent_end(reward);
}

void on_agent_cleanup(rlSocket theSocket)
{
  agent_cleanup();

  /*
    deallocate( g_observation );
    deallocate( g_action );
  */
}

void on_termination(rlSocket theSocket)
{
  free(theTaskSpecBuffer);
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
