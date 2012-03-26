/** Header **/

#include "model/Peer.h"

namespace SAYL {

/**********************************************************************************************
 * Constructor
 *********************************************************************************************/
Peer::Peer (const std::string __address, int __port)
  : address (__address), port (__port)
{
  // ...
}

/**********************************************************************************************
 * getAddress
 *********************************************************************************************/
std::string Peer::getAddress () const {
  return address;
}

/**********************************************************************************************
 * getAddress
 *********************************************************************************************/
int Peer::getPort () const {
  return port;
}

/**********************************************************************************************
 * Comparaison operators
 *********************************************************************************************/
bool Peer::operator == (const Peer & peer) const {
  return address == peer.address && port == peer.port;
}

bool Peer::operator != (const Peer & peer) const {
  return address != peer.address || port != peer.port;
}


bool Peer::operator < (const Peer & peer) const {
  return address < peer.address || port < peer.port;
}

bool Peer::operator <= (const Peer & peer) const {
  return address <= peer.address || port <= peer.port;
}


bool Peer::operator > (const Peer & peer) const {
  return address > peer.address || port > peer.port;
}

bool Peer::operator >= (const Peer & peer) const {
  return address >= peer.address || port >= peer.port;
}

} // namespace SAYL

