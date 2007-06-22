#ifndef mines_h

#include <RL_common.h>

Task_specification env_init();
Observation env_start();
Reward_observation env_step(Action a);
void env_cleanup();
void env_set_state(State_key sk);
void env_set_random_seed(Random_seed_key rsk);
State_key env_get_State();
Random_seed_key env_get_random_seek();
char* env_message(const char* inMessage);

#endif