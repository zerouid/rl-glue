#ifndef mines_h

#include <RLcommon.h>

Task_specification env_init();
Observation env_start();
Reward_observation env_step(Action a);

#endif
