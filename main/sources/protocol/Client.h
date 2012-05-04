/** Header **/

#ifndef __SAYL_CLIENT_H__
#define __SAYL_CLIENT_H__

#include "network/ClientSocket.h"

namespace SAYL {

/**********************************************************************************************
 * Client
 *********************************************************************************************/
class Client : public ClientSocket {
public :
  /** Constructor **/
  Client ();
  
  /** Desctructor **/
  ~Client ();
  /** GetPieces **/
  virtual ResponseData GetPieces (RequestGetPieces &,File& )=0;
  
  /** Interested **/
  virtual ResponseHave Interested (RequestInterested &,File& )=0;
  
  /** Look **/
  //virtual ResponseFile Look (RequestLook &,File& )=0;
  /**Have*/
  virtual ResponseHave Have(RequestHave&,File& )=0;
protected :

  /** handleData **/  
  virtual void handleData (ResponseData &,File&);

  /** handleHave **/
  virtual void handleHave (ResponseHave &);

  /** handleList **/
  virtual void handleList (ResponseList &);

  /** handleOK **/
  virtual void handleOK (ResponseOK &);

  /** handlePeers **/
  virtual void handlePeers (ResponsePeers &);
private:
  	 Server& localserver;
}; // class Client

} // namespace SAYL

#endif

