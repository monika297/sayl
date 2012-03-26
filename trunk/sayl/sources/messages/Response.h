/** Header **/

#ifndef __SAYL_RESPONSE_H__
#define __SAYL_RESPONSE_H__

#define RESPONSE 0x0200
#define RESPONSE_DATA   RESPONSE | 1
#define RESPONSE_HAVE   RESPONSE | 2
#define RESPONSE_LIST   RESPONSE | 4
#define RESPONSE_OK     RESPONSE | 8
#define RESPONSE_PEERS  RESPONSE | 16

#include <string>
#include <vector>

#include <inttypes.h>

#include "messages/Message.h"

namespace SAYL {

/**********************************************************************************************
 * Response
 *********************************************************************************************/
class Response : public Message {
public :
  /** Constructor **/
  Response (std::string __address, int __port) : Message (__address, __port) {}
  virtual ~Response(){}
}; // class Response

/**********************************************************************************************
 * ResponseData
 *********************************************************************************************/
class ResponseData : public Response {
public:
  /** Constructor **/
  ResponseData (std::string, int, int);
  ~ResponseData();
  
  /** getPieceSize **/
  int getPieceSize () const;

  /** getKey **/
  std::string getKey () const;

  /** setKey **/
  void setKey (std::string);

  /* Piece */

  /** getPieceCount **/
  int getPieceCount () const;

  /** getPieceIndex **/
  int getPieceIndex (int) const;
  
  /** getPieceData **/
  const uint8_t * getPieceData (int) const;
  
  /** addPiece **/
  bool addPiece (int, uint8_t *);
  
  /** getCommand **/
  int getCommand () const {
    return RESPONSE_DATA;
  }
  
  /** getCommandName **/
  std::string getCommandName () const {
    return "data";
  }

private :
  /** Piece **/
  struct Piece {
    int index;
    uint8_t * content;
  };

  int piecesize;
  std::vector <Piece> pieces;
  std::string key;

}; // class ResponseData


/**********************************************************************************************
 * ResponseHave
 *********************************************************************************************/
class ResponseHave : public Response {
public:
  /** Constructor **/
  ResponseHave (std::string, int, int);
  
  /** Destructor **/
  ~ResponseHave ();

  /** getKey **/
  std::string getKey () const;

  /** setKey **/
  void setKey (std::string);

  /** getBufferMapSize **/
  int getBufferMapSize () const;

  /** getBufferMap **/
  const uint8_t * getBufferMap () const;

  /** setBufferMap **/
  bool setBufferMap (uint8_t *);
  
  /** getCommand **/
  int getCommand () const {
    return RESPONSE_HAVE;
  }
  
  /** getCommandName **/
  std::string getCommandName () const {
    return "have";
  }

private :
  int size;
  std::string key;
  uint8_t * buffermap;

}; // class ResponseHave


/**********************************************************************************************
 * ResponseList
 *********************************************************************************************/
class ResponseList : public Response {
public:
  /** Constructor **/
  ResponseList (std::string __address, int __port) : Response (__address, __port) {}
  virtual ~ResponseList(){}
  /* File */

  /** getFileCount **/
  int getFileCount () const;
  
  /** getFileName **/
  std::string getFileName (int) const;
  
  /** getFileKey **/
  std::string getFileKey (int) const;
  
  /** getFileSize **/
  long getFileSize (int) const;
  
  /** getFilePieceSize **/
  long getFilePieceSize (int) const;

  /** addFile **/
  void addFile (std::string, std::string, long, long);
  
  /** getCommand **/
  int getCommand () const {
    return RESPONSE_LIST;
  }
  
  /** getCommandName **/
  std::string getCommandName () const {
    return "list";
  }

private :
  /** FileInfo **/
  struct FileInfo {
    std::string filename;
    std::string key;
    long filesize;
    long piecesize;
  }; // struct FileInfo

  std::vector <FileInfo> files;

}; // class ResponseList


/**********************************************************************************************
 * ResponseOK
 *********************************************************************************************/
class ResponseOK : public Response {
public:
  /** Constructor **/
  ResponseOK (std::string __address, int __port) : Response (__address, __port) {}
  
  /** getCommand **/
  int getCommand () const {
    return RESPONSE_OK;
  }
  
  /** getCommandName **/
  std::string getCommandName () const {
    return "ok";
  }

}; // class ResponseOK


/**********************************************************************************************
 * ResponsePeers
 *********************************************************************************************/
class ResponsePeers : public Response {
public:
  /** Constructor **/
  ResponsePeers (std::string __address, int __port) : Response (__address, __port) {}
  virtual ~ResponsePeers(){}
  /** getKey **/
  std::string getKey () const;

  /** setKey **/
  void setKey (std::string);

  /* Peers */
  
  /** getPeersCount **/
  int getPeersCount () const;
  
  /** getPeerAddress **/
  std::string getPeerAddress (int) const;
  
  /** getPeerPort **/
  int getPeerPort (int) const;
  
  /** addPeed **/
  void addPeer (std::string, int);
  
  /** getCommand **/
  int getCommand () const {
    return RESPONSE_PEERS;
  }
  
  /** getCommandName **/
  std::string getCommandName () const {
    return "peers";
  }

private :
  struct PeerInfo {
    std::string address;
    int port;
  };

  std::vector <PeerInfo> peers;
  std::string key;

}; // class ResponsePeers


}  // namespace SAYL

#endif

