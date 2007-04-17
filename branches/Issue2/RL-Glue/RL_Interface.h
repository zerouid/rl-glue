#include "stdlib.h"
#include "stdio.h"
#include <vector>
#include "RLcommon.h"

#ifndef RL_INTERFACE_H
#define RL_INTERFACE_H

typedef struct {
	Reward r;
	Observation o;
	Action a;
	bool terminal;
} Reward_observation_action_terminal;

typedef struct {
	Observation o;
	Action a;
} Observation_action;

#ifdef PIPES
void RL_init();
void RL_init(char* inpipe, char* outpipe);
#else
void RL_init();
#endif

Observation_action RL_start();
Reward_observation_action_terminal RL_step();
Reward RL_return();
Reward RL_average_reward();
double RL_average_num_steps();
int RL_num_steps();
int RL_num_episodes();
void RL_episode();
void RL_episode(int num_Steps);
void RL_freeze_agent();

#ifdef PIPES
void RL_set_state(State_key sk);
void RL_set_random_seed(Random_seed_key rsk);
void RL_set_state(int sk);
void RL_set_state(float sk);
void RL_set_state(std::vector <int>& sk);
void RL_set_state(std::vector <float>& sk);
void RL_set_random_seed(int rsk);
void RL_set_random_seed(float rsk);
void RL_set_random_seed(std::vector <int>& rsk);
void RL_set_random_seed(std::vector <float>& rsk);
#else
void RL_set_state(State_key sk);
void RL_set_random_seed(Random_seed_key rsk);
#endif

State_key RL_get_state();
Random_seed_key RL_get_random_seed();
void RL_cleanup();

#ifdef PIPES
void agent_init(Task_specification task_);
void agent_init(Task_specification task_, char* inpath, char* outpath);
#endif



#ifdef PIPES
Task_specification env_init();
Task_specification env_init(char* inpath, char* outpath);
#endif


#endif
