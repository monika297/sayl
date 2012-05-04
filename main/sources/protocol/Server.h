/** Header **/
#include "network/ServerSocket.h"
#include "services/Tracker.h"
#include "messages/Request.h"
#include "messages/Message.h"
#include "messages/Response.h"
#ifndef __SAYL_SERVER_H__
#define __SAYL_SERVER_H__


class Tracker;
namespace SAYL {

/**********************************************************************************************
 * Server
 *********************************************************************************************/
class Server : public ServerSocket {
public :
  /** Constructor **/
  Server ();

  /** Desctructor **/
  virtual ~Server ();

  /** Run **/
  bool Run ();

  /** TransportProtocol **/
  bool setTransportProtocol (std::string transport) {
    if (transport == "tcp") ServerSocket::setTransportProtocol ("tcp");
    else return false;
  }
    /** handleAnnounce **/
  virtual ResponseOK handleAnnounce (RequestAnnounce &){}

    /** handleGetFile **/
    virtual ResponsePeers handleGetFile (RequestGetFile &){}

    /** handleLook **/
    virtual ResponseList handleLook (RequestLook &){}

    /** handleUpdate **/
    virtual ResponseOK handleUpdate (RequestUpdate &){}
protected:
  Directory<File> files;
}; // class Server

} // namespace SAYL

#endif

