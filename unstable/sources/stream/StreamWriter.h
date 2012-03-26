/** Header **/

#ifndef __SAYL_STREAM_WRITER_H__
#define __SAYL_STREAM_WRITER_H__

#include <inttypes.h> /* Integer types definitions */

#include "network/Socket.h"

#include "messages/Message.h"
#include "messages/Request.h"
#include "messages/Response.h"

namespace SAYL {

/**********************************************************************************************
 * StreamWriter
 *********************************************************************************************/
class StreamWriter {
public :
  /** Constructor **/
  StreamWriter (Socket*);
  
  /** Desctructor **/
  ~StreamWriter ();

  /** Write **/
  bool Write (const Message &);  

private :

  /* Requests */
  
  /** WriteRequestAnnounce **/
  bool WriteRequestAnnounce (const RequestAnnounce*);

  /** WriteRequestGetFile **/
  bool WriteRequestGetFile (const RequestGetFile*);

  /** WriteRequestGetPieces **/
  bool WriteRequestGetPieces (const RequestGetPieces*);

  /** WriteRequestInterested **/
  bool WriteRequestInterested (const RequestInterested*);

  /** WriteRequestLook **/
  bool WriteRequestLook (const RequestLook*);

  /** WriteRequestUpdate **/
  bool WriteRequestUpdate (const RequestUpdate*);

  /* Responses */

  /** WriteResponseData **/
  bool WriteResponseData (const ResponseData*);

  /** WriteResponseHave **/
  bool WriteResponseHave (const ResponseHave*);

  /** WriteRequestList **/
  bool WriteResponseList (const ResponseList*);

  /** WriteResponseOK **/
  bool WriteResponseOK (const ResponseOK*);

  /** WriteResponsePeers **/
  bool WriteResponsePeers (const ResponsePeers*);

  Socket * socket;
    
}; // class StreamWriter

} // namespace SAYL

#endif

