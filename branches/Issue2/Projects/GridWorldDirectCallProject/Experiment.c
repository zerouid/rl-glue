/********************************************************************************
 *  Experiment.cpp
 *  
 *  Created by Adam White, created on March 29 2007.
 *  Copyright (c) 2005 UAlberta. All rights reserved.
 *
 *  Description.
 *
 *		Experiment (and driver) program. Specifies the experimental 
 *		setup for a learning experiment. 
 *
 *		This experiment program reports online and asymototic performance 
 *		measures based on 100 independent runs. For the online performance, 
 *		the agent in trained for 100 episodes and its average reward per episode 
 *		is recorded. For the aysmtotic performance, the agent is trained for 
 *		10000 episodes, then its policy is frooze and its average reward per 
 *		episode over 100 episodes is recorded. 
 ********************************************************************************/

#include "RL_Interface.h"
#include <stdio.h>
#include "math.h"

double trainAgent(int training_time);
double testAgent(int test_length);


int main(int argc, char *argv[])
{
		int training_time = 100;
		int test_length = 100;
		int longrun_training_time = 10000;
		int phase2_training = longrun_training_time -training_time; 
		int num_runs = 100;
		
		double mean_training_reward = 0;
		double mean_testing_reward = 0;
		double training_se=0;
		double testing_se=0;

		double training_reward[num_runs];
		double testing_reward[num_runs];


		for(int x=0; x < num_runs; x++)
		{
		
			srand48(x*100);

			RL_init();					//initialize interface
			
			training_reward[x] = trainAgent(training_time);
			trainAgent(phase2_training);
			testing_reward[x] = testAgent(test_length);

			mean_training_reward += training_reward[x];
			mean_testing_reward += testing_reward[x];
			
			RL_cleanup();				//de-initialize interface

			printf(".");
			fflush(stdout);
		}

		mean_training_reward = mean_training_reward/(double)num_runs;
		mean_testing_reward = mean_testing_reward/(double)num_runs;
		
		double sum1=0, sum2=0;
		for(int x=0; x<num_runs; x++)
		{
			sum1 += (training_reward[x] - mean_training_reward)*(training_reward[x] - mean_training_reward);
			sum2 += (testing_reward[x] - mean_testing_reward)*(testing_reward[x] - mean_testing_reward);
		}

		training_se = sqrt((1.0/(num_runs-1.0))*sum1);
		testing_se = sqrt((1.0/(num_runs-1.0))*sum2);
				
			
		printf("\n**************************************************************\n");
		printf("Online learning performance, over %d episodes, is %10.2f reward per episode. Result averaged over %d runs. Standard error = %10.2f\n", training_time, mean_training_reward,num_runs,training_se); 
		printf("Agent trained for %d episodes. Asymptotic performance, over %d episodes, is %10.2f reward per episode. Result averaged over %d runs. Standard error = %10.2f\n",longrun_training_time, test_length, mean_testing_reward, num_runs,testing_se); 
		printf("**************************************************************\n");
	
}

double trainAgent(int training_time)
{
	double cumulative_reward = 0;
	for(int y=0; y < training_time; y++)
	{
		RL_episode();	//run a single episode
		cumulative_reward += RL_return();
	}
	return cumulative_reward/(training_time + 0.0);
}

double testAgent(int test_length)
{
	RL_freeze_agent();
	
	double cumulative_reward = 0;
	for(int y=0; y < test_length; y++)
	{
		RL_episode(1000);	//run a single episode
		cumulative_reward += RL_return();
	}
	return cumulative_reward/(test_length + 0.0);
}



