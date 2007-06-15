from RL_netlib import *
import sys

sock = None

def RL_init():
	global sock
	sock = waitForConnection(kLocalHost,kDefaultPort,kRetryTimeout)
	sock.sendInt(kExperimentConnection)
	sock.sendInt(kRLInit)
	sys.stderr.write('sent rl_init to rl_glue\n')

def RL_start():
	sock.sendInt(kRLStart)
	sys.stderr.write('sending rl_start to rl_glue\n')
	return sock.recvObservationAction()

def RL_step():
	sock.sendInt(kRLStep)
	return sock.recvRewardObservationActionTerm()

def RL_cleanup():
	sock.sendInt(kRLCleanup)
	sock.close()

def RL_return():
	sock.sendInt(kRLReturn)
	return sock.recvDouble()

def RL_num_steps():
	sock.sendInt(kRLNumSteps)
	return sock.recvInt()
	
def RL_num_episodes():
	sock.sendInt(kRLNumEpisodes)
	return sock.recvInt()

def RL_episode(num_steps):
	sock.sendInt(kRLEpisode)
	sock.sendInt(num_steps)

def RL_set_state(theStateKey):
	sock.sendInt(kRLSetState)
	sock.sendADT(theStateKey)

def RL_set_random_seed(theRandomSeedKey):
	sock.sendInt(kRLSetRandomSeed)
	sock.sendADT(theRandomSeedKey)

def RL_get_state():
	sock.sendInt(kRLGetState)
	theStateKey = sock.recvADT()
	return theStateKey

def RL_get_random_seed():
	sock.sendInt(kRLGetRandomSeed)
	theRandomSeedKey = sock.recvADT()
	return theRandomSeedKey