from RL_common import *
import random
import sys

zeroAgentAction = Action()
zeroAgentObservation = Observation()

def agent_init(taskSpec):
	random.seed(0)
	zeroAgentAction.numInts = 1
	zeroAgentAction.intArray = [1]
	sys.stderr.write('agent_init\n')

def agent_start(observation):
	zeroAgentObservation = observation
	zeroAgentAction.intArray = [random.randint(0,2)]
	sys.stderr.write('agent_start\n')
	return zeroAgentAction

def agent_step(reward, observation):
	zeroAgentObservation = observation
	zeroAgentAction.intArray = [random.randint(0,2)]
	sys.stderr.write('agent_step\n')
	return zeroAgentAction

def agent_end(reward):
	sys.stderr.write('agent_end\n')

def agent_cleanup():
	pass