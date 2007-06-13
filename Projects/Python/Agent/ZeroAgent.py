from RL_common import *
import random
import sys

zeroAgentAction = Action()
zeroAgentObservation = Observation()

def agent_init(taskSpec):
	random.seed(0)
	zeroAgentAction.numInts = 1
	zeroAgentAction.intArray = [1]

def agent_start(observation):
	zeroAgentObservation = observation
	zeroAgentAction.intArray = [random.randint(0,2)]
	return zeroAgentAction

def agent_step(reward, observation):
	zeroAgentObservation = observation
	zeroAgentAction.intArray = [random.randint(0,2)]
	return zeroAgentAction

def agent_end(reward):
	pass

def agent_cleanup():
	pass