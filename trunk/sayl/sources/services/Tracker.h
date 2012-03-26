/** Header **/

#ifndef __SAYL_TRACKER_H__
#define __SAYL_TRACKER_H__
#include "model/Directory.h"
#include "protocol/Server.h"


namespace SAYL {
/**********************************************************************************************
 * Tracker
 *********************************************************************************************/
class Tracker : public Server {
public :
  /** Constructor **/
  Tracker ();

  /** Desctructor **/
  virtual ~Tracker ();

  /** handleAnnounce **/
  ResponseOK handleAnnounce (RequestAnnounce &);

  /** handleGetFile **/
  ResponsePeers handleGetFile (RequestGetFile &);

  /** handleLook **/
  ResponseList handleLook (RequestLook &);

  /** handleUpdate **/
  ResponseOK handleUpdate (RequestUpdate &);

}; // class Tracker

} // namespace SAYL

#endif

