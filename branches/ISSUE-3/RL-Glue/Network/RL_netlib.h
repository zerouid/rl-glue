#ifndef libSocketRL_h
#define libSocketRL_h

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

int rlGetSystemByteOrder();

#endif
