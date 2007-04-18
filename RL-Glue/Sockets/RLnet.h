#ifndef libSocketRL_h
#define libSocketRL_h

/* Data types */

typedef struct rlSocket_t
{
  short port;
  int os_socket;
} rlSocket;

rlSocket rlOpen(short port);
rlSocket rlAcceptConnection(rlSocket socket);

int rlIsValidSocket(rlSocket socket);
int rlConnect(rlSocket socket, const char* address);
int rlListen(rlSocket);
int rlClose(rlSocket socket);

int rlSendData(rlSocket socket, const void* data, int length);
int rlRecvData(rlSocket socket, void* data, int length);

int rlGetSystemByteOrder();

#endif
