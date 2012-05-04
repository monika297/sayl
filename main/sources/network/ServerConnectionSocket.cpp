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

#include "network/ServerConnectionSocket.h"

namespace SAYL {

/**********************************************************************************************
 * Constructeur
 *********************************************************************************************/ 
ServerConnectionSocket::ServerConnectionSocket (ServerSocket * socket, int descriptor)
 : Socket (descriptor)
{
  start = 0;
  remoteaddress = "";
  remoteport = 0;

  // Initialization
  server = socket;
  if (server != (ServerSocket *) 0) {
    start = server->getElapsedTime ();
  }
}
  
/**********************************************************************************************
 * Destructor
 *********************************************************************************************/
ServerConnectionSocket::~ServerConnectionSocket () { }

/**********************************************************************************************
 * getElapsed (sec)
 *********************************************************************************************/
double ServerConnectionSocket::getElapsed () const {
  if (isconnected && server != 0) {
    double time = server->getElapsedTime () - start;
    if (time > 0.) return time;
  }
  return 0.;
}

}  // namespace SAYL

