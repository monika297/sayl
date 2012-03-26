/** Header **/

#ifndef __SAYL_PEER_H__
#define __SAYL_PEER_H__

#include <string>

namespace SAYL {

/**********************************************************************************************
 * Peer
 *********************************************************************************************/
class Peer {
public :
  /** Constructor **/
  Peer (const std::string, int);
  
  /** getAddress **/
  std::string getAddress () const;

  /** getPort **/
  int getPort () const;

  /** Comparaison Operator **/
  bool operator == (const Peer &) const;
  bool operator != (const Peer &) const;

  bool operator <  (const Peer &) const;
  bool operator <= (const Peer &) const;

  bool operator >  (const Peer &) const;
  bool operator >= (const Peer &) const;

private :
  /* File attributes */
  std::string address;
  int port;

}; // class Peer

}  // namespace SAYL

#endif

