/** Header **/
#ifndef __SAYL_CLIENT_SOCKET_H__
#define __SAYL_CLIENT_SOCKET_H__

#include "Service.h"
#include "network/Socket.h"

namespace SAYL {

/**********************************************************************************************
 * ClientSocket
 *********************************************************************************************/
class ClientSocket : public Service, public Socket {
public :
  /** Constructor **/
  ClientSocket (std::string, std::string, int) ;
  
  /** Destructor **/
  virtual ~ClientSocket ();
  
  /** Run **/
  virtual bool Run () {
    return Service::Run ();
  }
  
  /** getElapsedTime (sec) **/
  double getElapsedTime () const {
    return Service::getElapsedTime ();
  }

private :
  /** socket **/
  int clientsocket;
  
}; // class ClientSocket

} // namespace SAYL

#endif

