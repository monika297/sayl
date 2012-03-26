/** Header **/
#include <string>
#include <iostream>
#include <sstream>

#include "StreamWriter.h"

namespace SAYL {

/**********************************************************************************************
 * Constructeur
 *********************************************************************************************/ 
StreamWriter::StreamWriter (Socket* connection) : socket (connection) {}

/**********************************************************************************************
 * Desctructor
 *********************************************************************************************/ 
StreamWriter::~StreamWriter () {}

/**********************************************************************************************
 * Write
 *********************************************************************************************/
bool StreamWriter::Write (const Message & message) {
  switch (message.getCommand ()) {
    case REQUEST_ANNOUNCE :
      return WriteRequestAnnounce ((RequestAnnounce *) &message);

    case REQUEST_GETFILE :
      return WriteRequestGetFile ((RequestGetFile*) &message);

    case REQUEST_GETPIECES :
      return WriteRequestGetPieces ((RequestGetPieces*) &message);

    case REQUEST_INTERESTED :
      return WriteRequestInterested ((RequestInterested*) &message);

    case REQUEST_LOOK :
      return WriteRequestLook ((RequestLook*) &message);

    case REQUEST_UPDATE :
      return WriteRequestUpdate ((RequestUpdate*) &message);

    case RESPONSE_DATA :
      return WriteResponseData ((ResponseData*) &message);

    case RESPONSE_HAVE :
      return WriteResponseHave ((ResponseHave*) &message);

    case RESPONSE_LIST :
      return WriteResponseList ((ResponseList*) &message);

    case RESPONSE_OK :
      return WriteResponseOK ((ResponseOK*) &message);

    case RESPONSE_PEERS :
      return WriteResponsePeers ((ResponsePeers*) &message);
    
    default :
      return false;
  }
}

/**********************************************************************************************
 * WriteRequestAnnounce
 *********************************************************************************************/ 
bool StreamWriter::WriteRequestAnnounce (const RequestAnnounce* request) {
  if (socket == (Socket *) 0 || ! socket->isConnected ()) {
    return false;
  }

  std::ostringstream stream;
  stream << "announce listen " << request->getPort () << " seed [";
  for (int index = 0; index < request->getSeedCount (); index++) {
    if (index > 0) stream << " ";
    stream << "\"" << request->getSeedFileName (index) << "\" "
           << request->getSeedFileSize (index) << " "
           << request->getSeedPieceSize (index) << " "
           << request->getSeedKey (index);
  }
  stream << "] leech [";
  for (int index = 0; index < request->getLeechCount (); index++) {
    if (index > 0) stream << " ";
    stream << request->getLeechKey (index);
  }
  stream << "] ";

  std::string content = stream.str ();
  return (socket->Write ((uint8_t*)content.data (), content.length ()));
}

/**********************************************************************************************
 * WriteRequestGetFile
 *********************************************************************************************/ 
bool StreamWriter::WriteRequestGetFile (const RequestGetFile* request) {
  if (socket == (Socket *) 0 || ! socket->isConnected ()) {
    return false;
  }

  std::ostringstream stream;
  stream << "getfile " << request->getKey () << " ";

  std::string content = stream.str ();
  return (socket->Write ((uint8_t*)content.data (), content.length ()));
}

/**********************************************************************************************
 * WriteRequestGetPieces
 *********************************************************************************************/ 
bool StreamWriter::WriteRequestGetPieces (const RequestGetPieces* request) {
  if (socket == (Socket *) 0 || ! socket->isConnected ()) {
    return false;
  }

  std::ostringstream stream;
  stream << "getpieces " << request->getKey () << " [";
  for (int index = 0; index < request->getPieceIndexCount (); index++) {
    if (index > 0) stream << " ";
    stream << request->getPieceIndex (index);
  }
  stream << "] ";

  std::string content = stream.str ();
  return (socket->Write ((uint8_t*)content.data (), content.length ()));
}

/**********************************************************************************************
 * WriteRequestInterested
 *********************************************************************************************/ 
bool StreamWriter::WriteRequestInterested (const RequestInterested* request) {
  if (socket == (Socket *) 0 || ! socket->isConnected ()) {
    return false;
  }

  std::ostringstream stream;
  stream << "interested " << request->getKey () << " ";

  std::string content = stream.str ();
  return (socket->Write ((uint8_t*)content.data (), content.length ()));
}

/**********************************************************************************************
 * WriteRequestLook
 *********************************************************************************************/ 
bool StreamWriter::WriteRequestLook (const RequestLook* request) {
  if (socket == (Socket *) 0 || ! socket->isConnected ()) {
    return false;
  }

  std::ostringstream stream;
  stream << "look [";
  for (int index = 0; index < request->getConditionsCount (); index++) {
    if (index > 0) stream << " ";
    stream << request->getConditionLeftOperand (index) << " "
           << request->getConditionOperator (index)    << " "
           << request->getConditionRightOperand (index);
  }
  stream << "] ";
  std::string content = stream.str ();
  return (socket->Write ((uint8_t*)content.data (), content.length ()));
}

/**********************************************************************************************
 * WriteRequestUpdate
 *********************************************************************************************/ 
bool StreamWriter::WriteRequestUpdate (const RequestUpdate* request) {
  if (socket == (Socket *) 0 || ! socket->isConnected ()) {
    return false;
  }

  std::ostringstream stream;
  stream << "update seed [";
  for (int index = 0; index < request->getLeechCount (); index++) {
    if (index > 0) stream << " ";
    stream << request->getSeed (index);
  }
  stream << "] leech [";
  for (int index = 0; index < request->getLeechCount (); index++) {
    if (index > 0) stream << " ";
    stream << request->getLeech (index);
  }
  stream << "] ";

  std::string content = stream.str ();
  return (socket->Write ((uint8_t*)content.data (), content.length ()));
}

/**********************************************************************************************
 * WriteResponseData
 *********************************************************************************************/ 
bool StreamWriter::WriteResponseData (const ResponseData* request) {
  if (socket == (Socket *) 0 || ! socket->isConnected ()) {
    return false;
  }
  
  std::ostringstream stream (std::ios_base::binary);
  stream << "data " << request->getKey () << " [";
  for (int index = 0; index < request->getPieceCount (); index++) {
    if (index > 0) stream << " ";
    stream << request->getPieceIndex (index) << ":";
    stream.write ((const char *) request->getPieceData (index), request->getPieceSize ());
  }
  stream << "] ";

  std::string content = stream.str ();
  return (socket->Write ((uint8_t*) content.data (), content.length ()));
}

/**********************************************************************************************
 * WriteResponseHave
 *********************************************************************************************/ 
bool StreamWriter::WriteResponseHave (const ResponseHave* request) {
  if (socket == (Socket *) 0 || ! socket->isConnected ()) {
    return false;
  }

  std::ostringstream stream (std::ios_base::binary);
  stream << "have " << request->getKey () << " ";
  stream.write ((const char *) request->getBufferMap (), request->getBufferMapSize ());
  stream << " ";
  std::string content = stream.str ();
  return (socket->Write ((uint8_t*) content.data (), content.length ()));
}

/**********************************************************************************************
 * WriteResponseList
 *********************************************************************************************/ 
bool StreamWriter::WriteResponseList (const ResponseList* request) {
  if (socket == (Socket *) 0 || ! socket->isConnected ()) {
    return false;
  }

  std::ostringstream stream;
  stream << "list [";
  for (int index = 0; index < request->getFileCount (); index++) {
    if (index > 0) stream << " ";
    stream << "\"" << request->getFileName (index) << "\" "
           << request->getFileSize (index) << " "
           << request->getFilePieceSize (index) << " "
           << request->getFileKey (index);
  }
  stream << "] ";

  std::string content = stream.str ();
  return (socket->Write ((uint8_t*)content.data (), content.length ()));
}

/**********************************************************************************************
 * WriteResponseOK
 *********************************************************************************************/ 
bool StreamWriter::WriteResponseOK (const ResponseOK* request) {
  if (socket == (Socket *) 0 || ! socket->isConnected ()) {
    return false;
  }
  return (socket->Write ((uint8_t*) "ok ", 3));
}

/**********************************************************************************************
 * WriteResponsePeers
 *********************************************************************************************/ 
bool StreamWriter::WriteResponsePeers (const ResponsePeers* request) {
  if (socket == (Socket *) 0 || ! socket->isConnected ()) {
    return false;
  }

  std::ostringstream stream;
  stream << "peers " << request->getKey () << " [";
  for (int index = 0; index < request->getPeersCount (); index++) {
    if (index > 0) stream << " ";
    stream << request->getPeerAddress (index) << ":" << request->getPeerPort (index);
  }
  stream << "] ";

  std::string content = stream.str ();
  return (socket->Write ((uint8_t*)content.data (), content.length ()));
}

}  // namespace SAYL

