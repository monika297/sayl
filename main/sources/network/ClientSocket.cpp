/** Header **/
#include <iostream>
#include <string>

#include "network/ClientSocket.h"

#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>

namespace SAYL {

/**********************************************************************************************
 * Constructor
 *********************************************************************************************/ 
ClientSocket::ClientSocket (std::string __protocol, std::string __remote_address, int __remote_port) {
  if (__protocol == "udp") clientsocket = socket (AF_INET, SOCK_DGRAM, 0);
  else clientsocket = socket (AF_INET, SOCK_STREAM, 0);
  
  if (clientsocket < 0) {
    std::cerr << "Could not create client socket." << std::endl;
  }
  
  struct sockaddr_in address;
  bzero (&address, sizeof (struct sockaddr_in));
  
  address.sin_family = AF_INET;
  address.sin_port = htons(__remote_port);
  
  struct hostent * remote = gethostbyname (__remote_address.c_str ());
  if (remote == 0) {
    std::cerr << "Could not resolve remote server address ..." << std::endl;
  }
  bcopy((char *) remote->h_addr, (char *) &address.sin_addr.s_addr, remote->h_length);
  
  if (connect(clientsocket,(struct sockaddr*) &address, sizeof(struct sockaddr_in)) < 0){
    std::cerr << "Could not connect socket ..." << std::endl;
  }
  
  isconnected = true;
  fcntl (clientsocket, F_SETFL, O_NONBLOCK);
  resetTime ();

  Socket::setDescriptor (clientsocket);
}
  
/**********************************************************************************************
 * Destructor
 *********************************************************************************************/
ClientSocket::~ClientSocket () {
  Close ();
}

}  // namespace SAYL
