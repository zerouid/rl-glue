#ifndef RL_netlib_h
#define RL_netlib_h

/* Defines types for RL-Glue */
#include <RL_common.h>

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
