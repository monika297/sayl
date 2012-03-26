/** Header **/

#ifndef __SAYL_REQUEST_H__
#define __SAYL_REQUEST_H__

#define REQUEST 0x0100
#define REQUEST_ANNOUNCE    REQUEST | 1
#define REQUEST_GETFILE     REQUEST | 2
#define REQUEST_GETPIECES   REQUEST | 4
#define REQUEST_HAVE        REQUEST | 8
#define REQUEST_INTERESTED  REQUEST | 16
#define REQUEST_LOOK        REQUEST | 32
#define REQUEST_UPDATE      REQUEST | 64

#include <string>
#include <vector>

#include <inttypes.h>

#include "messages/Message.h"
#include "Criterion.h"

namespace SAYL {

/**********************************************************************************************
 * Request
 *********************************************************************************************/
class Request : public Message {
public :
  /** Constructor **/
  Request (std::string __address, int __port) : Message (__address, __port) {}
  virtual ~Request(){}
}; // class Request

/**********************************************************************************************
 * RequestAnnounce
 *********************************************************************************************/
class RequestAnnounce : public Request {
public:
  /** Constructor **/
  RequestAnnounce (std::string __address, int __port) : Request (__address, __port), port (0) {}

  /** getPort **/
  int getPort () const;

  /** setPort **/
  void setPort (int);
  
  /* Seed */
  
  /** getSeedCount **/
  int getSeedCount () const;
  
  /** getSeedFileName **/
  std::string getSeedFileName (int) const;
  
  /** getSeedKey **/
  std::string getSeedKey (int) const;
  
  /** getSeedFileSize **/
  long getSeedFileSize (int) const;
  
  /** getSeedPieceSize **/
  long getSeedPieceSize (int) const;
  
  /** addSeed **/
  void addSeed (std::string, std::string, long, long);
  
  /* Leech */
  
  /** getLeechCount **/
  int getLeechCount () const;
  
  /** getLeech **/
  std::string getLeechKey (int) const;
  
  /** addLeech **/
  void addLeech (std::string);
  
  /** getCommand **/
  int getCommand () const {
    return REQUEST_ANNOUNCE;
  }
  
  /** getCommandName **/
  std::string getCommandName () const {
    return "announce";
  }

private :
  /** FileInfo **/
  struct FileInfo {
    std::string filename;
    std::string key;
    long filesize;
    long piecesize;
  }; // struct FileInfo

  int port;
  std::vector <FileInfo> seed;
  std::vector <std::string> leech;

}; // class RequestAnnounce

/**********************************************************************************************
 * RequestGetFile
 *********************************************************************************************/
class RequestGetFile : public Request {
public:
  /** Constructor **/
  RequestGetFile (std::string __address, int __port) : Request (__address, __port) {}

  /** getKey **/
  std::string getKey () const;

  /** setKey **/
  void setKey (std::string);
  
  /** getCommand **/
  int getCommand () const {
    return REQUEST_GETFILE;
  }
  
  /** getCommandName **/
  std::string getCommandName () const {
    return "getfile";
  }

private :
  std::string key;

}; // class RequestGetFile


/**********************************************************************************************
 * RequestGetPieces
 *********************************************************************************************/
class RequestGetPieces : public Request {
public:
  /** Constructor **/
  RequestGetPieces (std::string __address, int __port) : Request (__address, __port) {}

  /** getKey **/
  std::string getKey () const;

  /** setKey **/
  void setKey (std::string);
  
  /* Piece */

  /** getPieceIndexCount **/
  int getPieceIndexCount () const;
  
  /** getPieceIndex **/
  int getPieceIndex (int) const;
  
  /** addPieceIndex **/
  void addPieceIndex (int);
  
  /** getCommand **/
  int getCommand () const {
    return REQUEST_GETPIECES;
  }
  
  /** getCommandName **/
  std::string getCommandName () const {
    return "getpieces";
  }

private :
  std::string key;
  std::vector <int> pieces;

}; // class RequestGetPieces


/**********************************************************************************************
 * RequestHave
 *********************************************************************************************/
class RequestHave : public Request {
public:
  /** Constructor **/
  RequestHave (std::string, int, int);
  
  /** Destructor **/
  virtual ~RequestHave ();

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
    return REQUEST_HAVE;
  }
  
  /** getCommandName **/
  std::string getCommandName () const {
    return "have";
  }

private :
  int size;
  std::string key;
  uint8_t * buffermap;

}; // class RequestHave


/**********************************************************************************************
 * RequestInterested
 *********************************************************************************************/
class RequestInterested : public Request {
public:
  /** Constructor **/
  RequestInterested (std::string __address, int __port) : Request (__address, __port) {}

  /** getKey **/
  std::string getKey () const;

  /** setKey **/
  void setKey (std::string);
  
  /** getCommand **/
  int getCommand () const {
    return REQUEST_INTERESTED;
  }
  
  /** getCommandName **/
  std::string getCommandName () const {
    return "interested";
  }

private :
  std::string key;

}; // class RequestInterested


/**********************************************************************************************
 * RequestLook
 *********************************************************************************************/
class RequestLook : public Request {
public:
  /** Constructor **/
  RequestLook (std::string __address, int __port) : Request (__address, __port) {}

  /** getConditionsCount **/
  int getConditionsCount () const;

  /** getConditionOperator **/
  Comparator::Operator getConditionOperator (int index) const;

  /** getConditionLeftOperand **/
  std::string getConditionLeftOperand (int) const;

  /** getConditionRightOperand **/
  std::string getConditionRightOperand (int) const;
  
  /** addCondition **/
  void addCondition (Comparator::Operator ,std::string, std::string);
  /** conditionCheckFile */
  bool conditionCheckFile(int,File&);

  /** getCommand **/
  int getCommand () const {
    return REQUEST_LOOK;
  }
  
  /** getCommandName **/
  std::string getCommandName () const {
    return "look";
  }

private :
  /** struct Condition **/
  std::vector <Criterion> conditions;

}; // class RequestLook


/**********************************************************************************************
 * RequestUpdate
 *********************************************************************************************/
class RequestUpdate : public Request {
public:
  /** Constructor **/
  RequestUpdate (std::string __address, int __port) : Request (__address, __port) {}
  virtual ~RequestUpdate(){}
  /* Seed */
  
  /** getSeedCount **/
  int getSeedCount () const;
  
  /** getSeed **/
  std::string getSeed (int) const;
  
  /** addSeed **/
  void addSeed (std::string);
  
  /* Leech */
  
  /** getLeechCount **/
  int getLeechCount () const;
  
  /** getLeech **/
  std::string getLeech (int) const;
  
  /** addLeech **/
  void addLeech (std::string);
  
  /** getCommand **/
  int getCommand () const {
    return REQUEST_UPDATE;
  }
  
  /** getCommandName **/
  std::string getCommandName () const {
    return "update";
  }

private :
  std::vector <std::string> seed;
  std::vector <std::string> leech;

}; // class RequestUpdate

}  // namespace SAYL

#endif

