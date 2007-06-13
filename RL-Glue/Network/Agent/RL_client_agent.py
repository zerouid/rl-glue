import sys
import time
from RL_agent import *
from RL_netlib import *

def onAgentInit(sock):
	theTaskSpecLength = sock.recvInt()
	taskSpec = ''
	if theTaskSpecLength > 0:
		taskSpec = sock.recvString(theTaskSpecLength)
	agent_init(taskSpec)

def onAgentStart(sock):
	theObservation = sock.recvADTHeader()
	theObservation = sock.recvADTBody(theObservation)
	theAction = agent_start(theObservation)
	sock.sendADT(theAction)

def onAgentStep(sock):
	theReward = sock.recvDouble()
	theObservation = sock.recvADTHeader()
	theObservation = sock.recvADTBody(theObservation)
	theAction = agent_step(theReward, theObservation)
	sock.sendADT(theAction)

def onAgentEnd(sock):
	reward = sock.recvDouble()
	agent_end(reward)

def onAgentCleanup(sock):
	agent_cleanup()

def runAgentEventLoop(sock):
	agentState = 0
	while agentState != kAgentCleanup:
		agentState = sock.recvInt()
		if agentState == kAgentInit:
			onAgentInit(sock)
		elif agentState == kAgentStart:
			onAgentStart(sock)
		elif agentState == kAgentStep:
			onAgentStep(sock)
		elif agentState == kAgentEnd:
			onAgentEnd(sock)
		elif agentState == kAgentCleanup:
			onAgentCleanup(sock)
			return
		else:
			sys.stderr.write(kUnknownMessage % (agentState))

while True:
	sock = waitForConnection(kLocalHost,kDefaultPort,kRetryTimeout)
	sock.sendInt(kAgentConnection)
	runAgentEventLoop(sock)
	sock.close()
