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

int rlGetSystemByteOrder() {
  /*
    Endian will be 1 when we are on a little endian machine,
    and not 1 on a big endian machine.
  */

  const int one = 1;
  const char endian = *(char*)&one;

  return endian;
}

void rlSwapData(unsigned char* out, const void* in, const unsigned int size) {
  const unsigned char *src = (const unsigned char *)&in;
  unsigned int i = 0;
  for (i = 0; i < size; ++i) {
    out[i] = src[size-i];
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


/* rlBuffer API */
void rlBufferCreate(rlBuffer *buffer, unsigned int capacity) {
  buffer->size     = 0;
  buffer->capacity = 0;
  
  if (capacity > 0) {
    rlBufferReseve(buffer, capacity);
  }
}

void rlBufferDestroy(rlBuffer *buffer) {
  free(buffer->data);
}

void rlBufferWrite(rlBuffer *buffer, unsigned char *data, unsigned int count, unsigned int size) {
  static int littleEndian = -1;

  unsigned int i = 0;
  unsigned char* data_ptr = 0;

  if (littleEndian == -1) {
    littleEndian = rlGetSystemByteOrder();
  }

  if (buffer->capacity < count * size + buffer->size) {
    rlBufferReserve(&buffer, count * size + buffer->size);
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

void rlBufferReserve(rlBuffer *buffer, unsigned int capacity) {
  unsigned char* new_data = 0;

  /* Ensure the buffer can hold the new data */
  if (capacity > buffer->capacity) {

    /* Allocate enough memory for the additional data */
    new_data = (unsigned char*)malloc(capacity + (capacity - buffer->capacity) * 2);

    /* Copy the existing data into the the larger memory allocation */
    memcpy(new_data, buffer->data, buffer->size);

    /* Free the original data */
    free(buffer->data);

    /* Set the buffers data to the new data pointer */
    buffer->data = new_data;

    /* Set the new capacity */
    buffer->capacity = capacity + (capacity - buffer->capacity) * 2;
  }
}

void rlBufferSendData(rlSocket theSocket, const void* sendData, unsigned int count, unsigned int sendTypeSize) {
  const unsigned char* data = (const unsigned char*)sendData;
  rlBuffer buffer = {0};
  
  rlBufferCreate(&buffer, sizeof(unsigned int) + count * sendTypeSize);
  rlBufferWrite(&buffer, &buffer->size, 1, sizeof(unsigned int));
  rlBufferWrite(&buffer + sizeof(unsigned int), data, count, sendTypeSize);
  rlSendData(socket, buffer->data, buffer->size);
  rlBufferDestroy(&buffer);
}

void rlBufferRecvData(rlSocket theSocket, void* recvData, unsigned int recvTypeSize) {
  unsigned int count = 0;  
  rlBuffer buffer = {0};

  rlBufferCreate(&buffer, sizeof(unsigned int));
  rlRecvData(socket, buffer->data, sizeof(unsigned int));
  rlBufferRead(&buffer, 0, &count, 1, sizeof(unsigned int));
  rlBufferClear(&buffer);

  rlBufferReserve(&buffer, count);
  rlRecvData(socket, buffer->data, count * recvTypeSize);
  rlBufferRead(&buffer, 0, recvData, count, recvTypeSize);
  rlBufferDestroy(&buffer);
}

void rlBufferSendADT(rlSocket socket, RL_abstract_type *data) {
  rlBuffer buffer = {0};
  
  rlBufferCreate(&buffer, sizeof(unsigned int * 2) + data->numInts * sizeof(int) + data->numDoubles * sizeof(double));
  rlBufferWrite(&buffer, &data->numInts, 1, sizeof(unsigned int));
  rlBufferWrite(&buffer, &data->numDoubles, 1, sizeof(unsigned int));
  rlBufferWrite(&buffer, data->intArray, data->numInts, sizeof(int));
  rlBufferWrite(&buffer, data->doubleArray, data->numDoubles, sizeof(double));
  rlSendData(socket, buffer->data, buffer->size);
  rlBufferDestroy(&buffer);
}

void rlBufferRecvADT(rlSocket socket, RL_abstract_type *data) {
  unsigned int recvSize = 0;
  rlBuffer buffer = {0};

  rlBufferCreate(&buffer, sizeof(unsigned int * 2));
  rlRecvData(socket, buffer->data, sizeof(unsigned int) * 2);
  rlBufferRead(&buffer, 0, &data->numInts, 1, sizeof(unsigned int));
  rlBufferRead(&buffer, sizeof(unsigned int), &data->numDoubles, 1, sizeof(unsigned int));
  rlBufferClear(&buffer);

  /* we assume that the ADT is = {0} on initalization! */
  if (data->intArray == 0 && data->doubleArray == 0) {
    if (data->numInts > 0) {
      data->intArray = (int*)calloc(data->numInts, sizeof(int));
    }
    if (data->numDoubles > 0) {
      data->doubleArray = (double*)calloc(data->numDoubles, sizeof(double));
    }
  }
  
  rlRecvData(socket, buffer->data, data->numInts * sizeof(int) + data->numDoubles * sizeof(double));
  rlBufferRead(&buffer, 0, data->intArray, data->numInts, sizeof(int));
  rlBufferRead(&buffer, sizeof(int) * data->numInts, data->doubleArray, data->numDoubles, sizeof(double));

  rlBufferDestroy(&buffer);
}
