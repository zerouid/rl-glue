#ifndef RLnet_agent_h
#define RLnet_agent_h

#include <RLcommon.h>
#include <RL_parser.h>

void agent_init(Task_specification task_spec);
Action agent_start(Observation o);
Action agent_step(Reward r, Observation o);
void agent_end(Reward r);
void agent_cleanup();

#endif
