/* Standard Headers */
#include <assert.h> /* assert */
#include <stdlib.h> /* calloc */
#include <string.h> /* memset */

/* Network Headers */
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

/* RL_netlib Library Header */
#include <Network/RL_netlib.h>

extern void rlSetAgentConnection(int);
extern void rlSetEnvironmentConnection(int);
extern void rlSetExperimentConnection(int);

void rlSwapData(unsigned char* out, const void* in, const unsigned int size) {
  const unsigned char *src = (const unsigned char *)&in;
  unsigned int i = 0;
  for (i = 0; i < size; ++i) {
    out[i] = src[size-i];
  }
}

void rlCreateBuffer(rlBuffer *buffer, unsigned int capacity) {
  buffer->size     = 0;
  buffer->capacity = capacity;
  
  if (capacity > 0) {
    buffer->data = (unsigned char*)calloc(capacity, sizeof(unsigned char*));
  }
}

void rlDestroyBuffer(rlBuffer *buffer) {
  free(buffer->data);
}

void rlBufferWrite(rlBuffer *buffer, unsigned char *data, unsigned int count, unsigned int size) {
  static int littleEndian = -1;

  unsigned int i = 0;
  unsigned char* new_data = 0;
  unsigned char* data_ptr = 0;

  if (littleEndian == -1) {
    littleEndian = rlGetSystemByteOrder();
  }
  
  /* Ensure the buffer can hold the new data */
  if (count * size > buffer->capacity - buffer->size) {

    /* Allocate enough memory for the additional data */
    new_data = (unsigned char*)malloc((count * size) + (buffer->size * 2));

    /* Copy the existing data into the the larger memory allocation */
    memcpy(new_data, buffer->data, buffer->size);

    /* Free the original data */
    free(buffer->data);

    /* Set the buffers data to the new data pointer */
    buffer->data = new_data;

    /* Set the new capacity */
    buffer->capacity = (count * size) + (buffer->size * 2);
  }

  /* Get the offset to the place in the buffer we want to start inserting */
  data_ptr = buffer->data + buffer->size;

  /* For each of the new data items, swap the endianness and add them to the buffer */
  for (i = 0; i < count; ++i) {
    if (littleEndian) {
      rlSwapData(&data_ptr[i * size], &data[i * size], size);
    }
    else {
      memcpy(&data_ptr[i * size], &data[i * size], size);
    }
  }

  buffer->size += count * size;
}

void rlBufferRead(rlBuffer *buffer, unsigned int offset, unsigned char *data, unsigned int count, unsigned int size) {
  static int littleEndian = -1;   
  unsigned int i = 0;

  if (littleEndian == -1) {
    rlGetSystemByteOrder();
  }

  /* For each of the new data items, swap the endianness and read them from the buffer */
  for (i = 0; i < count; ++i) {
    if (littleEndian) {
      rlSwapData(&data[i * size], &buffer->data[(i * size) + offset], size);
    }
    else {
      memcpy(&data[i * size], &buffer->data[(i * size) + offset], size);
    }
  }
}

void rlBufferClear(rlBuffer *buffer) {
  buffer->size = 0;
}

rlSocket rlOpen(short thePort) {
  int flag = 1;
  rlSocket theSocket = 0;

  theSocket = socket(PF_INET, SOCK_STREAM, 0);
  setsockopt(theSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(int));

  return theSocket;
}

rlSocket rlAcceptConnection(rlSocket theSocket) {
  rlSocket theClient = 0;
  struct sockaddr_in theClientAddress = {0};
  unsigned int theSocketSize = sizeof(struct sockaddr_in);
  theClient = accept(theSocket, (struct sockaddr*)&theClientAddress, &theSocketSize);
  return theClient;
}

int rlConnect(rlSocket theSocket, const char* theAddress, short thePort) {
  int theStatus = 0;
  struct sockaddr_in theDestination;
  theDestination.sin_family = AF_INET;
  theDestination.sin_port = htons(thePort);
  theDestination.sin_addr.s_addr = inet_addr(theAddress);
  memset(&theDestination.sin_zero, '\0', 8);

  theStatus = connect(theSocket, 
		      (struct sockaddr*)&theDestination, 
		      sizeof(struct sockaddr));

  return theStatus;
}

int rlListen(rlSocket theSocket, short thePort) {
  struct sockaddr_in theServer;
  int theStatus = 0;
  int yes = 1;
  
  theServer.sin_family = AF_INET;
  theServer.sin_port = htons(thePort);
  theServer.sin_addr.s_addr = INADDR_ANY;
  memset(&theServer.sin_zero, '\0', 8);
  
  /* We don't really care if this fails... */
  setsockopt(theSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

  theStatus = bind(theSocket, 
		   (struct sockaddr*)&theServer, 
		   sizeof(struct sockaddr));

  if (theStatus == -1) return -1;
  
  theStatus = listen(theSocket, 10);
  if (theStatus == -1) return -1;

  return theStatus;
}

int rlClose(rlSocket theSocket) {
  return close(theSocket);
}

int rlIsValidSocket(rlSocket theSocket) {
  return theSocket != -1;
}

int rlSendData(rlSocket theSocket, const void* theData, int theLength) {
  int theBytesSent = 0;
  int theMsgError = 0;
  const char* theDataBuffer = (const char*)theData;
  
  while (theBytesSent < theLength) {
    theMsgError = send(theSocket, theDataBuffer + theBytesSent, theLength - theBytesSent, 0);
    if (theMsgError == -1) break;
    else theBytesSent += theMsgError;
  }

  return theBytesSent;
}

int rlRecvData(rlSocket theSocket, void* theData, int theLength) {
  int theBytesRecv = 0;
  int theMsgError = 0;
  char* theDataBuffer = (char*)theData;
  
  while (theBytesRecv < theLength) {
    theMsgError = recv(theSocket, theDataBuffer + theBytesRecv, theLength - theBytesRecv, 0);
    if (theMsgError <= 0) break;
    else theBytesRecv += theMsgError;
  }

  return theBytesRecv;
}

void rlSendADT(rlSocket socket, RL_abstract_type* data) {
  rlSendData(socket, &data->numInts, sizeof(int));
  rlSendData(socket, &data->numDoubles, sizeof(int));

  if (data->numInts > 0) {
    rlSendData(socket, data->intArray, sizeof(int) * data->numInts);
  }

  if (data->numDoubles > 0) {
    rlSendData(socket, data->doubleArray, sizeof(double) * data->numDoubles);
  }
}

void rlRecvADTHeader(rlSocket socket, RL_abstract_type* data) {
  rlRecvData(socket, &data->numInts, sizeof(int));
  rlRecvData(socket, &data->numDoubles, sizeof(int));
}

void rlRecvADTBody(rlSocket socket, RL_abstract_type* data) {
  if (data->numInts > 0) {
    rlRecvData(socket, data->intArray, sizeof(int) * data->numInts);
  }

  if (data->numDoubles > 0) {
    rlRecvData(socket, data->doubleArray, sizeof(double) * data->numDoubles);
  }
}

RL_abstract_type* rlAllocADT(RL_abstract_type *data, unsigned int numInts, unsigned int numDoubles) {
  if (data != 0) {
    data->numInts    = numInts;
    data->numDoubles = numDoubles;
    if (numInts > 0) {
      data->intArray = (int*)calloc(data->numInts, sizeof(int));
    }
    if (numDoubles > 0) {
      data->doubleArray = (double*)calloc(data->numDoubles, sizeof(double));
    }
  }

  return data;
}

void rlFreeADT(RL_abstract_type *data) {
  if (data != 0) {
    free(data->intArray);
    free(data->doubleArray);

    data->numInts     = 0;
    data->numDoubles  = 0;
    data->intArray    = 0;
    data->doubleArray = 0;
  }
}

rlSocket rlWaitForConnection(const char *address, const short port, const int retryTimeout) {
  rlSocket theConnection = 0;
  int isConnected = -1;

  while(isConnected == -1) {
    theConnection = rlOpen(port);
    assert(rlIsValidSocket(theConnection));
    isConnected = rlConnect(theConnection, address, port);
    if (isConnected == -1) { 
      rlClose(theConnection);
      sleep(retryTimeout);
    }
  }

  return theConnection;
}

int rlGetSystemByteOrder() {
  /*
    Endian will be 1 when we are on a little endian machine,
    and not 1 on a big endian machine.
  */

  const int one = 1;
  const char endian = *(char*)&one;

  return endian;
}
