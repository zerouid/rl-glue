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
#define kAgentFreeze  30
#define kAgentMessage 31

#define kEnvInit          9
#define kEnvStart         10
#define kEnvStep          11
#define kEnvCleanup       12
#define kEnvSetState      13
#define kEnvSetRandomSeed 14
#define kEnvGetState      15
#define kEnvGetRandomSeed 16
#define kEnvMessage       32

#define kRLInit           17
#define kRLStart          18
#define kRLStep           19
#define kRLCleanup        20
#define kRLReturn         21
#define kRLNumSteps       22
#define kRLNumEpisodes    23
#define kRLEpisode        24
#define kRLSetState       25
#define kRLSetRandomSeed  26
#define kRLGetState       27
#define kRLGetRandomSeed  28
#define kRLFreeze         29

#define kLocalHost "127.0.0.1"
#define kDefaultPort 4096
#define kRetryTimeout 10

#define ntohll(x) (((long long)(ntohl((int)((x << 32) >> 32))) << 32) | (unsigned int)ntohl(((int)(x >> 32))))
#define htonll(x) ntohll(x)


/* Data types */
typedef int rlSocket;

typedef struct rlBuffer_t {
  unsigned int size;
  unsigned int capacity;
  unsigned char *data;
} rlBuffer;

/* rlBuffer API */
void rlSwapData(unsigned char* out, const void* in, const unsigned int size);
void rlCreateBuffer(rlBuffer *buffer, unsigned int capacity);
void rlDestroyBuffer(rlBuffer *buffer);
void rlBufferWrite(rlBuffer *buffer, unsigned char *data, unsigned int count, unsigned int size);
void rlBufferRead(rlBuffer *buffer, unsigned int offset, unsigned char *data, unsigned int count, unsigned int size);
void rlBufferClear(rlBuffer *buffer);

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
void rlAllocADT(RL_abstract_type *data, unsigned int numInts, unsigned int numDoubles);
void rlFreeADT(RL_abstract_type *data);

rlSocket rlWaitForConnection(const char *address, const short port, const int retryTimeout);

int rlGetSystemByteOrder();


#endif
