/** Header **/
#ifndef __SAYL_SERVER_SOCKET_H__
#define __SAYL_SERVER_SOCKET_H__

#include "Service.h"
#include <iostream>
#include <vector>
#include "ConnectionSocket.h"
namespace SAYL {

/**********************************************************************************************
 * ServerSocket
 *********************************************************************************************/
class ServerSocket : public Service {
public :
  /** Constructor **/
  ServerSocket ();
  
  /** Destructor **/
  virtual ~ServerSocket () {}

  /** Run **/
  virtual bool Run ();
  
  /** isOpen **/
  bool isOpen () const;

  /** Connections **/
  int getConnectionsCount () const;
  ConnectionSocket* getConnection (unsigned int) const;
  
  /** SimultaneousConnections **/
  int getSimultaneousConnections () const;
  bool setSimultaneousConnections (int);

  /** LocalPort **/
  int getLocalPort () const;
  virtual bool setLocalPort (int);

  /** TransportProtocol **/
  std::string getTransportProtocol () const;
  virtual bool setTransportProtocol (std::string transport);

  /** Transfered (byte) **/
  long getTransfered () const;

  /** Received (byte) **/
  long getReceived () const;

  /** TransferRate (byte/sec) **/
  double getTransferRate () const;

  /** ReceptionRate (byte/sec) **/
  double getReceptionRate () const;
  
protected :  
  /** Open **/
  bool Open ();
  
  /** Close **/
  bool Close ();
  
  /** AcceptConnection **/
  int AcceptConnection ();
  
private :
  /** Properties **/
  std::string protocol;
  int port;
  int simultaneous;
  bool isopen;
  
  /** Socket **/
  int serversocket;

  /** Connections **/
  std::vector <ConnectionSocket> connections;

}; // class ServerSocket

} // namespace SAYL

#endif

