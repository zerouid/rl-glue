
#ifndef MINEAGENT_H
#define MINEAGENT_H

#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "C_agent_parser.h"

// -------- Required code -------------
#include "RLcommon.h"

void agent_init(Task_specification task_);
Action agent_start(Observation o);
Action agent_step(Reward r, Observation o);
void agent_end(Reward r);  
void agent_cleanup();
void agent_freeze();
// ------------------------------------

Action act;

int egreedy(int s);
int argmax(int s); 
// *******************************************************************************************
//tabular sarsa agent, no discounting

int n; 			//number of states
int m; 			//number of actions
int nm; 		//the length of the state-action value array
int A; 			//index of last action taken
int S; 			//index of last state/sensation observed
double **Q;  		//array of action values
int *actions;		//array of possible actions
double alpha; 		//step-size parameter
double epsilon; 	//exploration probability parameter

int state_dim;		//array holding size of each state dimension
int action_dim;	//array holding size of each action dimension
float version;		//version of interface
// *******************************************************************************************
#endif
