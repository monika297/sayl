/** Header **/
#include <iostream>
#include <vector>

#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>

#include "network/ServerSocket.h"
#include "network/ConnectionSocket.h"

namespace SAYL {

/**********************************************************************************************
 * Constructor
 *********************************************************************************************/ 
ServerSocket::ServerSocket () : Service () {
  protocol = "tcp";
  port = 0;
  simultaneous = 1;
  isopen = false;
}

/**********************************************************************************************
 * Run
 *********************************************************************************************/
bool ServerSocket::Run () {
  if (! Service::Run ()) return false;
  if (! isOpen ()) {
    if (! Open ()) return false; 
  }
  return true;
}

/**********************************************************************************************
 * Open
 *********************************************************************************************/
bool ServerSocket::Open () {  
  if (protocol == "udp") serversocket = socket (AF_INET, SOCK_DGRAM, 0);
  else serversocket = socket (AF_INET, SOCK_STREAM, 0);
  
  if (serversocket < 0) {
    std::cerr << "Could not create server socket." << std::endl;
  }
  
  struct sockaddr_in address;  
  bzero (&address, sizeof (struct sockaddr_in));
  
  address.sin_family = AF_INET;
  address.sin_port = htons(port);
  
  connections.clear ();
  connections.reserve(simultaneous);
  for (int index = 0; index < simultaneous; index++) {
    connections.push_back (ConnectionSocket (*this, 0));
  }
  
  if (bind(serversocket,(struct sockaddr*) &address, sizeof(struct sockaddr_in)) < 0){
    std::cerr << "Could not bind to socket for input ..." << std::endl;
    return false;
  }
  
  fcntl (serversocket, F_SETFL, O_NONBLOCK);
  
  if (protocol != "udp") { 
    if (listen (serversocket, simultaneous) < 0) {
      std::cerr << "Could not listen to the server socket" << std::endl;
      return false;
    }
  }
  
  isopen = true;
  resetTime ();
  return true;
}

/**********************************************************************************************
 * Close
 *********************************************************************************************/
bool ServerSocket::Close () {
  for (unsigned int index = 0; index < connections.size (); index++) {
    connections [index].Close ();
  }
  if (port != 0) shutdown (serversocket, 2);
  return true;
}

/**********************************************************************************************
 * AcceptConnection
 *********************************************************************************************/
int ServerSocket::AcceptConnection () {
  if (protocol != "udp") {
    struct sockaddr_in client;
    int length = sizeof (struct sockaddr_in);
    int connection = accept (serversocket, (struct sockaddr*) &client, (socklen_t*) &length);
    if (connection <= 0) {
      return -1;

    } else {
      for (unsigned int index = 0; index < connections.size (); index++) {
        if (! connections [index].isConnected ()) {
          connections [index] = ConnectionSocket (*this, connection);
          return index;
        }
      }
      return -1;
    }

  } else if (! connections [0].isConnected ()) {
    connections [0] = ConnectionSocket (*this, serversocket);
    return 0;    
  }
}

/**********************************************************************************************
 * Connections
 *********************************************************************************************/
int ServerSocket::getConnectionsCount () const {
  return connections.size ();
}

ConnectionSocket * ServerSocket::getConnection (unsigned int index) const {
  if (0 <= index && index < connections.size ()) return (ConnectionSocket *) &(connections[index]);
  else return 0;
}
  
/**********************************************************************************************
 * SimultaneousConnections
 *********************************************************************************************/
int ServerSocket::getSimultaneousConnections () const {
  return simultaneous;
}

bool ServerSocket::setSimultaneousConnections (int s) {
  if (s > 0) {
    simultaneous = s;
    return true;
  } else {
    return false;
  }
}

/**********************************************************************************************
 * LocalPort
 *********************************************************************************************/
int ServerSocket::getLocalPort () const {
  return port;
}

bool ServerSocket::setLocalPort (int p) {
  if (p > 0) {
    port = p;
    return true;
  } else {
    return false;
  }
}

/**********************************************************************************************
 * TansportProtocol
 *********************************************************************************************/
std::string ServerSocket::getTransportProtocol () const {
  return protocol;
}

bool ServerSocket::setTransportProtocol (std::string transport) {
  if (transport == "tcp" || transport == "udp") {
    protocol = transport;
    return true;
  } else {
    return false;
  }
}

/**********************************************************************************************
 * isOpen
 *********************************************************************************************/
bool ServerSocket::isOpen () const {
  return isopen;
}
   
/**********************************************************************************************
 * Transfered
 *********************************************************************************************/
long ServerSocket::getTransfered () const {
  long transfered = 0.;
  int count = getConnectionsCount ();
  for (int index = 0; index < count; index++) {
    transfered += connections[index].getTransfered ();
  }
  return transfered;
}

/**********************************************************************************************
 * Received
 *********************************************************************************************/
long ServerSocket::getReceived () const {
  long received = 0.;
  int count = getConnectionsCount ();
  for (int index = 0; index < count; index++) {
    received += connections[index].getReceived ();
  }
  return received;
}

/**********************************************************************************************
 * TransferRate
 *********************************************************************************************/
double ServerSocket::getTransferRate () const {
  double rate = 0.;
  int count = getConnectionsCount ();
  for (int index = 0; index < count; index++) {
    rate += connections[index].getTransferRate ();
  }
  return rate;
}

/**********************************************************************************************
 * ReceptionRate
 *********************************************************************************************/
double ServerSocket::getReceptionRate () const {
  double rate = 0.;
  int count = getConnectionsCount ();
  for (int index = 0; index < count; index++) {
    rate += connections[index].getReceptionRate ();
  }
  return rate;
}

} // namespace SAYL

