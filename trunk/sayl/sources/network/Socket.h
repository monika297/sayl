/** Header **/
#ifndef __SAYL_SOCKET_H__
#define __SAYL_SOCKET_H__

#include <inttypes.h>   /* Integer types definitions */
#include <string>
namespace SAYL {

/**********************************************************************************************
 * Socket
 *********************************************************************************************/
class Socket {
protected :
  /** Constructor **/
  Socket (int descriptor = 0) ;

public :
  /** Destructor **/
  virtual ~Socket ();

  /** Read **/
  bool Read (uint8_t *, int);

  /** Read **/
  bool Write (uint8_t *, int);

  /** Close **/
  void Close ();

  /** isConnected **/
  bool isConnected () const {
    return isconnected;
  }

  /** getTransfered (byte) **/
  long getTransfered () const {
    return transfered;
  }

  /** getReceived (byte) **/
  long getReceived () const {
    return received;
  }

  /** getRemoteAddress **/
  std::string getRemoteAddress () const {
    return remoteaddress;
  }

  /** getRemotePort **/
  int getRemotePort () const {
    return remoteport;
  }

  /** getLocalPort **/
  int getLocalPort () const {
    return localport;
  }

  /** getElapsedTime (sec) **/
  virtual double getElapsedTime () const = 0;

  /** getTransferRate (byte/sec) **/
  double getTransferRate () const {
    if (getElapsedTime () > 0.)  return transfered/getElapsedTime ();
    else return 0.;
  }

  /** getReceptionRate (byte/sec) **/
  double getReceptionRate () const {
    if (getElapsedTime () > 0.)  return received/getElapsedTime ();
    else return 0.;
  }

protected:
  /** setDescriptor **/
  void setDescriptor (int);

  bool isconnected;

  /** connection descriptor **/
  int connection;

  /** Properties **/
  long received;
  long transfered;

  std::string remoteaddress;
  long remoteport;
  long localport;

}; // class Socket

} // namespace SAYL

#endif
