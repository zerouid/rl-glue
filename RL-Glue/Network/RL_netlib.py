import socket
import struct
import array
import time
from RL_common import *

# RL-Glue needs to know what type of object is trying to connect.
kExperimentConnection  = 1
kAgentConnection       = 2
kEnvironmentConnection = 3

kAgentInit    = 4 # agent_* start by sending one of these values
kAgentStart   = 5 # to the client to let it know what type of
kAgentStep    = 6 # event to respond to
kAgentEnd     = 7
kAgentCleanup = 8

kEnvInit          = 9
kEnvStart         = 10
kEnvStep          = 11
kEnvCleanup       = 12
kEnvSetState      = 13
kEnvSetRandomSeed = 14
kEnvGetState      = 15
kEnvGetRandomSeed = 16

kRLInit           = 17
kRLStart          = 18
kRLStep           = 19
kRLCleanup        = 20
kRLReturn         = 21
kRLNumSteps       = 22
kRLNumEpisodes    = 23
kRLEpisode        = 24
kRLSetState       = 25
kRLSetRandomSeed  = 26
kRLGetState       = 27
kRLGetRandomSeed  = 28

kLocalHost = "127.0.0.1"
kDefaultPort = 4096
kRetryTimeout = 10

class rlSocket:
	
	def __init__(self):
		self.socket = None

	def open(self):
		try:
			self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
			self.socket.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
		except socket.error, msg:
			self.socket = None

	def acceptConnection(self):
		(self.conn, address) = self.socket.accept()
	
	def connect(self, address, port):
		try:
			self.socket.connect((address, port))
			return 1
		except socket.error, msg:
			return None
	
	def listen(self, port):
		self.socket.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR, 1)
		self.socket.bind(('',port))
		self.socket.listen(5)
	
	def close(self):
		self.socket.close()
	
	def isValidSocket(self):
		return self.socket == None
	
	def sendString(self, string):
		self.socket.sendall(string)
	
	def sendInt(self, data):
		packet = struct.pack('i',data)
		self.socket.sendall(packet)
	
	def sendDouble(self,data):
		packet = struct.pack('d',data)
		self.socket.sendall(packet)
	
	def sendInts(self, data):
		packet = array.array('l',data)
		self.socket.sendall(packet)

	def sendDoubles(self,data):
		packet = array.array('d',data)
		self.socket.sendall(packet)
	
	def recvString(self,length):
		s = ''
		while len(s) < length:
			s += self.socket.recv(length)
		return s
	
	def recvInt(self):
		s = ''
		while len(s) < 4:
			s += self.socket.recv(4)
		return struct.unpack('i',s)[0]
		
	def recvDouble(self):
		s = ''
		while len(s) < 8:
			s += self.socket.recv(8)
		return struct.unpack('d',s)[0]
	
	def recvInts(self,num):
		s = ''
		while len(s) < num*4:
			s += self.socket.recv(num*4)
		return struct.unpack("%di" % (num),s)

	def recvDoubles(self,num):
		s = ''
		while len(s) < num*8:
			s += self.socket.recv(num*8)
		return struct.unpack("%dd" % (num),s)

	def sendADT(self, data):
		self.sendInt(data.numInts)
		self.sendInt(data.numDoubles)
		if data.numInts > 0:
			self.sendInts(data.intArray)
		if data.numDoubles > 0:
			self.sendData(data.doubleArray)

	def recvADTHeader(self):
		data = RL_abstract_type()
		data.numInts = self.recvInt()
		data.numDoubles = self.recvInt()
		return data

	def recvADTBody(self,data):
		if data.numInts > 0:
			data.intArray = self.recvInts(data.numInts)
		if data.numDoubles > 0:
			data.doubleArray = self.recvDoubles(data.numDoubles)
		return data

def waitForConnection(address,port,retryTimeout):
	sock = rlSocket()
	isConnected = None
	while (isConnected == None):
		sock.open()
		isConnected = sock.connect(address,port)
		if (isConnected == None):
			sock.close()
			time.sleep(retryTimeout)
	return sock