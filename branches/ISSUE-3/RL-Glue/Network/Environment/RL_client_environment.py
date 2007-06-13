import sys
import time
from RL_environment import *
from RL_netlib import *

def onEnvInit(sock):
	taskSpec = env_init()
	theTaskSpecLength = len(taskSpec)
	
	sock.sendInt(theTaskSpecLength)
	if theTaskSpecLength > 0:
		sock.sendString(taskSpec)

def onEnvStart(sock):
	theObservation = env_start()
	sock.sendADT(theObservation)

def onEnvStep(sock):
	theAction = sock.recvADTHeader()
	theAction = sock.recvADTBody(theAction)
	
	ro = env_step(theAction)
	sock.sendDouble(ro.r)
	sock.sendADT(ro.o)
	sock.sendInt(ro.terminal)

def onEnvCleanup(sock):
	env_cleanup()

def onEnvSetState(sock):
	theStateKey = sock.recvADTHeader()
	theStateKey = sock.recvADTBody(theStateKey)
	env_set_state(theStateKey)
	
def onEnvSetRandomSeed(sock):
	theRandomSeed = sock.recvADTHeader()
	theRandomSeed = sock.recvADTBody(theRandomSeed)
	env_set_random_seed(theRandomSeed)

def onEnvGetState(sock):
	theStateKey = env_get_state()
	sock.sendADT(theStateKey)
	
def onEnvGetRandomSeed(sock):
	theRandomSeed = env_get_random_seed()
	sock.sendADT(theRandomSeed)

def runEnvironmentEventLoop(sock):
	envState = 0
	
	while envState != kEnvCleanup:
		envState = sock.recvInt()
		if envState == kEnvInit:
			onEnvInit(sock)
		elif envState == kEnvStart:
			onEnvStart(sock)
		elif envState == kEnvStep:
			onEnvStep(sock)
		elif envState == kEnvCleanup:
			onEnvCleanup(sock)
			return
		elif envState == kEnvSetState:
			onEnvSetState(sock)
		elif envState == kEnvSetRandomSeed:
			onEnvSetRandomSeed(sock)
		elif envState == kEnvGetState:
			onEnvGetState(sock)
		elif envState == kEnvGetRandomSeed:
			onEnvGetRandomSeed(sock)
		else:
			sys.stderr.write(kUnknownMessage % (agentState))

while True:
	sock = waitForConnection(kLocalHost,kDefaultPort,kRetryTimeout)
	sock.sendInt(kEnvironmentConnection)
	runEnvironmentEventLoop(sock)
	sock.close()
