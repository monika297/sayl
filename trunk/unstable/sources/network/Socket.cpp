/** Header **/
#include <iostream>
#include <string>

#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>

#include "network/Socket.h"

namespace SAYL {

/**********************************************************************************************
 * Constructor
 *********************************************************************************************/ 
Socket::Socket (int descriptor) {
  setDescriptor (descriptor);
}

/**********************************************************************************************
 * setDescriptor
 *********************************************************************************************/
void Socket::setDescriptor (int descriptor) {
  connection = 0;
  received = 0;
  transfered = 0;
  remoteaddress = "";
  remoteport = 0;

  // Initialization
  if (0 < descriptor) {
    char clientaddress [INET_ADDRSTRLEN] = "";
    int length = sizeof (struct sockaddr_in);
    struct sockaddr_in client;
    
    getsockname (descriptor, (struct sockaddr*) &client, (socklen_t*) &length);
    inet_ntop(AF_INET, (void*) &client.sin_addr, clientaddress, INET_ADDRSTRLEN);

    remoteaddress = std::string(clientaddress);
    remoteport = ntohl(client.sin_port);    
  }
}
  
/**********************************************************************************************
 * Destructor
 *********************************************************************************************/
Socket::~Socket () { }

/**********************************************************************************************
 * Close
 *********************************************************************************************/
void Socket::Close () {
  if (! isconnected) return ;
  shutdown (connection, 2);
  isconnected = false;
}

/**********************************************************************************************
 * Read
 *********************************************************************************************/
bool Socket::Read (uint8_t * content, int size) {
  if (! isconnected) return false;

  int receiveddata = recv (connection, (void*) content, size, MSG_DONTWAIT);
  if (receiveddata < 0) {
    if (errno != EAGAIN && errno != EWOULDBLOCK) {
      std::cerr << "An error occured while reading from socket (" << errno << ")" <<  std::endl;
    }
    return false;
    
  } else if (receiveddata == 0) {
    return false;
    
  } else {
    received += receiveddata;
    return true;
  }
}

/**********************************************************************************************
 * Write
 *********************************************************************************************/
bool Socket::Write (uint8_t * content, int size) {
  if (! isconnected) return false;

  int transfereddata = send (connection, (void*) content, size, 0);
  
  if (transfereddata < 0) {
    std::cerr << "An error occured while writing on socket." << std::endl;
    isconnected = false;
    return false;

  } else {
    transfered += transfereddata;
    // todo : check if receiveddata == size
    return true;
  }
  return false;
}

}  // namespace SAYL

