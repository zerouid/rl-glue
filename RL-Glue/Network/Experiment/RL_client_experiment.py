from RL_netlib import *

sock = None

def RL_init():
	global sock
	sock = waitForConnection(kLocalHost,kDefaultPort,kRetryTimeout)
	sock.sendInt(kExperimentConnection)
	sock.sendInt(kRLInit)

def RL_start():
	oa = Observation_action()
	sock.sendInt(kRLStart)
	theObservation = sock.recvADTHeader()
	theObservation = sock.recvADTBody(theObservation)
	theAction = sock.recvADTHeader()
	theAction = sock.recvADTBody(theAction)
	
	oa.o = theObservation
	oa.a = theAction
	return oa

def RL_step():
	roat = Reward_observation_action_terminal()
	sock.sendInt(kRLStep)
	roat.r = sock.recvDouble()
	roat.o = sock.recvADTHeader()
	roat.o = sock.recvADTBody(roat.o)
	roat.a = sock.recvADTHeader()
	roat.a = sock.recvADTBody(roat.a)
	roat.terminal = sock.recvInt()
	return roat

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
	theStateKey = sock.recvADTHeader()
	theStateKey = sock.recvADTBody(theStateKey)
	return theStateKey

def RL_get_random_seed():
	sock.sendInt(kRLGetRandomSeed)
	theRandomSeedKey = sock.recvADTHeader()
	theRandomSeedKey = sock.recvADTBody(theRandomSeedKey)
	return theRandomSeedKey