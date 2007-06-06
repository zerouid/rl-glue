#ifndef RL_netlib_h
#define RL_netlib_h

/* Defines types for RL-Glue */
#include <RL_common.h>

/* RL-Glue needs to know what type of object is trying to connect. */
#define kExperimentConnection  1
#define kAgentConnection       2
#define kEnvironmentConnection 3

#define kAgentInit    1 /* agent_* start by sending one of these values */
#define kAgentStart   2 /* to the client to let it know what type of    */
#define kAgentStep    3 /* event to respond to                          */
#define kAgentEnd     4
#define kAgentCleanup 5

/* Data types */
typedef int rlSocket;

/* Basic network functionality */
rlSocket rlOpen(short thePort);
rlSocket rlAcceptConnection(rlSocket theSocket);

int rlConnect(rlSocket theSocket, const char* theAddress, short thePort);
int rlListen(rlSocket theSocket, short thePort);
int rlClose(rlSocket theSocket);
int rlIsValidSocket(rlSocket theSocket);

int rlSendData(rlSocket socket, const void* data, int length);
int rlRecvData(rlSocket socket, void* data, int length);

void rlSendADT(rlSocket socket, RL_abstract_type* data);
void rlRecvADTHeader(rlSocket socket, RL_abstract_type* data);
void rlRecvADTBody(rlSocket socket, RL_abstract_type* data);

int rlGetSystemByteOrder();


#endif
