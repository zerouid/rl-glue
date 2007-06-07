#ifndef RL_netlib_h
#define RL_netlib_h

/* Defines types for RL-Glue */
#include <RL_common.h>

/* RL-Glue needs to know what type of object is trying to connect. */
#define kExperimentConnection  1
#define kAgentConnection       2
#define kEnvironmentConnection 3

#define kAgentInit    4 /* agent_* start by sending one of these values */
#define kAgentStart   5 /* to the client to let it know what type of    */
#define kAgentStep    6 /* event to respond to                          */
#define kAgentEnd     7
#define kAgentCleanup 8

#define kEnvInit          9
#define kEnvStart         10
#define kEnvStep          11
#define kEnvCleanup       12
#define kEnvSetState      13
#define kEnvSetRandomSeed 14
#define kEnvGetState      15
#define kEnvGetRandomSeed 16

#define kLocalHost "127.0.0.1"
#define kDefaultPort 4096
#define kRetryTimeout 10

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
void rlAllocADT(RL_abstract_type *data);
void rlFreeADT(RL_abstract_type *data);

int rlGetSystemByteOrder();


#endif