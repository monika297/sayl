/** Header **/
#ifndef __SAYL_SERVER_CONNECTION_SOCKET_H__
#define __SAYL_SERVER_CONNECTION_SOCKET_H__

#include "network/Socket.h"
#include "network/ServerSocket.h"

namespace SAYL {
class ServerSocket;
/**********************************************************************************************
 * ServerConnectionSocket
 *********************************************************************************************/
class ServerConnectionSocket : public Socket {
public :
  /** Constructor **/
  ServerConnectionSocket (ServerSocket * socket, int descriptor = 0) ;
  
  /** Destructor **/
  virtual ~ServerConnectionSocket ();
  
  /** getElapsed (sec) **/
  double getElapsed () const;
  
private :
  /** Server **/
  ServerSocket * server;
  double start;

}; // class ServerConnectionSocket

} // namespace SAYL

#endif

