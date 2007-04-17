#ifndef MINES_H
#define MINES_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
//---------------- REQUIRED ---------------------
#include "RLcommon.h"

Task_specification env_init();
Observation env_start();
Reward_observation env_step(Action a);
void env_cleanup();
void env_set_state(State_key sk);
void env_set_random_seed(Random_seed_key rsk);
State_key env_get_state();
Random_seed_key env_get_random_seed();
//-----------------------------------------------


int getPosition();
void getNextPosition(int action);
Reward getReward();  

typedef struct 
{
        int START;			//Start marker in grid
        int GOAL;			//End marker in grid
        int LAND;			//Free space in grid
        int OBSTACLE;	    //Obstical in grid
        int MINE;           //Mine in grid
        int row;			//Number of rows in grid
        int col;			//Number of columns in grid
        int startRow;		//Starting position
        int startCol;		//Starting position
        int agentRow;		//Agents current position
        int agentColumn;	//Agents current position
} mine_env;

		int env_map[6][19] = { 
        { 3, 3, 3, 3, 3, 3 ,3 ,3, 3, 3 ,3 ,3 ,3, 3, 3, 3, 3, 3}, 
        { 3,2,2,2,2,2,2,4,4,2,2,2,0,2,2,2,2,3 }, 
        { 3,2,2,2,2,4,2,2,2,2,2,2,2,2,2,2,2,3 },
        {3,2,2,2,2,2,2,2,2,4,4,4,2,2,2,2,3},
        { 3, 2,2,2,2,2,2,2,2,2,1,2,2,2,2,2,2,3},
        { 3, 3, 3, 3, 3, 3 ,3 ,3, 3, 3 ,3 ,3 ,3, 3, 3, 3, 3, 3}};
		
Observation o;
mine_env M;					//mine struct
Reward_observation ro;		//instance of observation type

bool env_terminal;

#endif
