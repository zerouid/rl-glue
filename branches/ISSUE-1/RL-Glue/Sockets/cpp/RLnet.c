/* Standard Headers */
#include <string.h>

/* Network Headers */
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

/* RLnet Library Header */
#include "RLnet.h"

rlSocket rlOpen(short thePort)
{
  rlSocket theSocket = {0};
  theSocket.os_socket = socket(PF_INET, SOCK_STREAM, 0);
  theSocket.port = thePort;
  return theSocket;
}

rlSocket rlAcceptConnection(rlSocket theSocket)
{
  rlSocket theClient = {0};
  struct sockaddr_in theClientAddress = {0};
  unsigned int theSocketSize = sizeof(struct sockaddr_in);

  theClient.os_socket = accept(theSocket.os_socket,
			       (struct sockaddr*)&theClientAddress,
			       &theSocketSize);

  return theClient;
}

int rlIsValidSocket(rlSocket theSocket)
{
  return theSocket.os_socket != -1;
}

int rlConnect(rlSocket theSocket, const char* theAddress)
{
  int theStatus = 0;
  struct sockaddr_in theDestination;
  theDestination.sin_family = AF_INET;
  theDestination.sin_port = htons(theSocket.port);
  theDestination.sin_addr.s_addr = inet_addr(theAddress);
  memset(&theDestination.sin_zero, '\0', 8);

  theStatus = connect(theSocket.os_socket, 
		      (struct sockaddr*)&theDestination, 
		      sizeof(struct sockaddr));

  return theStatus;
}

int rlListen(rlSocket theSocket)
{
  struct sockaddr_in theServer;
  int theStatus = 0;
  int yes = 1;
  
  theServer.sin_family = AF_INET;
  theServer.sin_port = htons(theSocket.port);
  theServer.sin_addr.s_addr = INADDR_ANY;
  memset(&theServer.sin_zero, '\0', 8);
  
  /* We don't really care if this fails... */
  setsockopt(theSocket.os_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

  theStatus = bind(theSocket.os_socket, 
		   (struct sockaddr*)&theServer, 
		   sizeof(struct sockaddr));

  if (theStatus == -1) return -1;
  
  theStatus = listen(theSocket.os_socket, 10);
  if (theStatus == -1) return -1;

  return theStatus;
}

int rlClose(rlSocket theSocket)
{
  return close(theSocket.os_socket);
}

int rlSendData(rlSocket theSocket, const char* theData, int theLength)
{
  int theBytesSent = 0;
  int theMsgError = 0;
  
  while (theBytesSent < theLength)
  {
    theMsgError = send(theSocket.os_socket, theData + theBytesSent, theLength - theBytesSent, 0);
    if (theMsgError == -1) break;
    else theBytesSent += theMsgError;
  }

  return theBytesSent;
}

int rlRecvData(rlSocket theSocket, char* theData, int theLength)
{
  int theBytesRecv = 0;
  int theMsgError = 0;

  while (theBytesRecv < theLength)
  {
    theMsgError = recv(theSocket.os_socket, theData + theBytesRecv, theLength - theBytesRecv, 0);
    if (theMsgError <= 0) break;
    else theBytesRecv += theMsgError;
  }

  return theBytesRecv;
}


int rlGetSystemByteOrder()
{
  // Endian will be 1 when we are on a little endian machine,
  // and not 1 on a big endian machine.

  static const int one = 1;
  static const char endian = (*(char*)(&one));

  return endian;
}
