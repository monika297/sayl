/** Header **/
#include <string>
#include <iostream>
#include <sstream>

#include <string.h>

#include "StreamReader.h"

namespace SAYL {

/**********************************************************************************************
 * Constructeur
 *********************************************************************************************/ 
StreamReader::StreamReader (Socket* connection)
  : socket (connection),
    token (TOKEN_NONE),
    state (STATE_INITIAL),
    current ('\0'),
    position (buffer),
    length (0),
    message ((Message *) 0)
{
}

/**********************************************************************************************
 * Desctructor
 *********************************************************************************************/ 
StreamReader::~StreamReader () {
  if (message != (Message *) 0) {
    delete message;
  }
}

/**********************************************************************************************
 * 
 * ReadMessage
 *
 * Message
 *   = Announce
 *   | Data
 *   | GetFile
 *   | GetPieces
 *   | Have
 *   | Interested
 *   | List
 *   | Look
 *   | OK
 *   | Peers
 *   | Update
 *   ;
 *
 *********************************************************************************************/ 
const Message * StreamReader::ReadMessage (int partsize) {
  if (socket == (Socket *) 0 || ! socket->isConnected ()) {
    return (Message *) 0;
  }

  if (message != (Message *) 0) {
    delete message;
  }
  message = (Message *) 0;
  state   = STATE_INITIAL;
  if (token != TOKEN_STREAM_END) {
    token   = TOKEN_NONE;
  }

  while (token != TOKEN_STREAM_END) {
    try {
             if (CurrentToken(TOKEN_ANNOUNCE   )) { message = ReadAnnounce   ();
      } else if (CurrentToken(TOKEN_GETFILE    )) { message = ReadGetFile    ();
      } else if (CurrentToken(TOKEN_GETPIECES  )) { message = ReadGetPieces  ();
      } else if (CurrentToken(TOKEN_INTERESTED )) { message = ReadInterested ();
      } else if (CurrentToken(TOKEN_LIST       )) { message = ReadList       ();
      } else if (CurrentToken(TOKEN_LOOK       )) { message = ReadLook       ();
      } else if (CurrentToken(TOKEN_PEERS      )) { message = ReadPeers      ();
      } else if (CurrentToken(TOKEN_UPDATE     )) { message = ReadUpdate     ();
      } else if (CurrentToken(TOKEN_OK         )) { message = ReadOK         ();
      } else if (CurrentToken(TOKEN_DATA       )) { message = ReadData       (partsize);
      } else if (CurrentToken(TOKEN_HAVE       )) { message = ReadHave       (partsize);
      } else if (CurrentToken(TOKEN_STREAM_END )) { return (Message *) 0;
      } else {
        SkipToken ();
        continue;
      }
      if (message != (Message *) 0) return message;
    } catch (...) {
      continue;
    }
  }
  return (Message *) 0;
}

/**********************************************************************************************
 *
 * ReadAnnounce
 *
 * Announce = ANNOUNCE LISTEN LITERAL_NUMERIC
 *            SEED  '[' { LITERAL_STRING LITERAL_NUMERIC LITERAL_NUMERIC LITERAL_STRING } ']'
 *            LEECH '[' { LITERAL_STRING } ']' ;
 *
 *********************************************************************************************/
Message * StreamReader::ReadAnnounce () {
  RequestAnnounce * request = new RequestAnnounce (socket->getRemoteAddress (),
                                                   socket->getRemotePort ());
  if (request == (RequestAnnounce *) 0)
    return (Message *) 0;

  try {
    AcceptToken (TOKEN_ANNOUNCE);
    AcceptToken (TOKEN_LISTEN);

    request->setPort (ReadNumericLiteral ());

    AcceptToken (TOKEN_SEED);
    AcceptToken (TOKEN_BRACKET_LEFT);
    
    while (CurrentToken (TOKEN_LITERAL_STRING)) {
      std::string filename = ReadStringLiteral  ();
      long filesize        = ReadNumericLiteral ();
      long piecesize       = ReadNumericLiteral ();
      std::string hashkey  = ReadStringLiteral  ();
      
      request->addSeed (filename, hashkey, filesize, piecesize);
    }
    
    AcceptToken (TOKEN_BRACKET_RIGHT);
    
    AcceptToken (TOKEN_LEECH);
    AcceptToken (TOKEN_BRACKET_LEFT);
    
    while (CurrentToken (TOKEN_LITERAL_STRING)) {
      request->addLeech (ReadStringLiteral ());
    }
    
    AcceptToken (TOKEN_BRACKET_RIGHT);
    
    return request;
    
  } catch (...) {
    if (request != (RequestAnnounce *) 0) delete request;
    return (Message *) 0;
  }
}

/**********************************************************************************************
 *
 * ReadData
 *
 * Data = DATA LITERAL_STRING '[' { LITERAL_NUMERIC ':' BINARY } ']' ;
 *
 *********************************************************************************************/ 
Message * StreamReader::ReadData (int partsize) {
  ResponseData * response = new ResponseData (socket->getRemoteAddress (),
                                              socket->getRemotePort (), partsize);
  if (response == (ResponseData *) 0)
    return (Message *) 0;

  uint8_t * data = (uint8_t *) 0;
  if (partsize > 0) {
    data = new uint8_t [partsize];
    if (data == (uint8_t *) 0) return (Message *) 0;
  }

  try {
    AcceptToken (TOKEN_DATA);

    response->setKey (ReadStringLiteral ());

    AcceptToken (TOKEN_BRACKET_LEFT);
    
    while (CurrentToken (TOKEN_LITERAL_NUMERIC)) {
      long index = ReadNumericLiteral ();
      ReadBinaryData (data, partsize);
      response->addPiece (index, data);
    }
    
    AcceptToken (TOKEN_BRACKET_RIGHT);

    delete [] data;
    
    return response;
    
  } catch (...) {
    if (response != (ResponseData *) 0) delete response;
    return (Message *) 0;
  }
}

/**********************************************************************************************
 *
 * ReadGetFile
 *
 * GetFile = GETFILE LITERAL_STRING ;
 *
 *********************************************************************************************/ 
Message * StreamReader::ReadGetFile () {
  RequestGetFile * request = new RequestGetFile (socket->getRemoteAddress (),
                                                 socket->getRemotePort ());
  if (request == (RequestGetFile *) 0)
    return (Message *) 0;

  try {
    AcceptToken (TOKEN_GETFILE);
      
    request->setKey (ReadStringLiteral ());
    return request;
    
  } catch (...) {
    if (request != (RequestGetFile *) 0) delete request;
    return (Message *) 0;
  }
}

/**********************************************************************************************
 *
 * ReadGetPieces
 *
 * GetPieces = GETPIECES LITERAL_STRING '[' { LITERAL_NUMERIC } ']' ;
 *
 *********************************************************************************************/ 
Message * StreamReader::ReadGetPieces () {
  RequestGetPieces * request = new RequestGetPieces (socket->getRemoteAddress (),
                                                     socket->getRemotePort ());
  if (request == (RequestGetPieces *) 0)
    return (Message *) 0;

  try {
    AcceptToken (TOKEN_GETPIECES);

    request->setKey (ReadStringLiteral ());

    AcceptToken (TOKEN_BRACKET_LEFT);    

    while (CurrentToken (TOKEN_LITERAL_NUMERIC)) {
      request->addPieceIndex (ReadNumericLiteral ());      
    }

    AcceptToken (TOKEN_BRACKET_RIGHT);
    
    return request;
    
  } catch (...) {
    if (request != (RequestGetPieces *) 0) delete request;
    return (Message *) 0;
  }
}

/**********************************************************************************************
 *
 * ReadHave
 *
 * Have = HAVE LITERAL_STRING BINARY ;
 *
 *********************************************************************************************/ 
Message * StreamReader::ReadHave (int partsize) {
  ResponseHave * response = new ResponseHave (socket->getRemoteAddress (),
                                              socket->getRemotePort (), partsize);
  if (response == (ResponseHave *) 0)
    return (Message *) 0;
    
  uint8_t * map = (uint8_t *) 0;
  if (partsize > 0) {
    map = new uint8_t [partsize];
    if (map == (uint8_t *) 0) return (Message *) 0;
  }

  try {
    AcceptToken (TOKEN_HAVE);

    response->setKey (ReadStringLiteral ());
    ReadBinaryData (map, partsize);
    response->setBufferMap (map);    
    delete [] map;
    
    return response;
    
  } catch (...) {
    if (response != (ResponseHave *) 0) delete response;
    return (Message *) 0;
  }
}

/**********************************************************************************************
 *
 * ReadInterested
 *
 * Interested = INTERESTED LITERAL_STRING ;
 *
 *********************************************************************************************/ 
Message * StreamReader::ReadInterested () {
  RequestInterested * request = new RequestInterested (socket->getRemoteAddress (),
                                                       socket->getRemotePort ());
  if (request == (RequestInterested *) 0)
    return (Message *) 0;

  try {
    AcceptToken (TOKEN_INTERESTED);

    request->setKey (ReadStringLiteral ());
    return request;
    
  } catch (...) {
    if (request != (RequestInterested *) 0) delete request;
    return (Message *) 0;
  }
}

/**********************************************************************************************
 *
 * ReadList
 *
 * List = LIST '[' { LITERAL_STRING LITERAL_NUMERIC LITERAL_NUMERIC LITERAL_STRING } ']' ;
 *
 *********************************************************************************************/ 
Message * StreamReader::ReadList () {
  ResponseList * response = new ResponseList (socket->getRemoteAddress (),
                                              socket->getRemotePort ());
  if (response == (ResponseList *) 0)
    return (Message *) 0;

  try {
    AcceptToken (TOKEN_LIST);
    AcceptToken (TOKEN_BRACKET_LEFT);    

    while (CurrentToken (TOKEN_LITERAL_STRING)) {      
      std::string filename = ReadStringLiteral ();
      long filesize = ReadNumericLiteral ();
      long piecesize = ReadNumericLiteral ();
      std::string hashkey = ReadStringLiteral ();
      
      response->addFile (filename, hashkey, filesize, piecesize);
    }    

    AcceptToken (TOKEN_BRACKET_RIGHT);

    return response;
    
  } catch (...) {
    if (response != (ResponseList *) 0) delete response;
    return (Message *) 0;
  }
}

/**********************************************************************************************
 *
 * ReadLook
 *
 * Look = LOOK '[' LITERAL_STRING '=' LITERAL_STRING ']' ;
 *
 *********************************************************************************************/ 
Message * StreamReader::ReadLook () {
  RequestLook * request = new RequestLook (socket->getRemoteAddress (),
                                           socket->getRemotePort ());
  if (request == (RequestLook *) 0)
    return (Message *) 0;

  try {
    AcceptToken (TOKEN_LOOK);
    AcceptToken (TOKEN_BRACKET_LEFT);
    
    while (CurrentToken (TOKEN_LITERAL_STRING)) {
      std::string __left = ReadStringLiteral ();
      Comparator::Operator __operator;
             if (CurrentToken (TOKEN_EQUAL           )) { __operator = Comparator::Equal ;
      } else if (CurrentToken (TOKEN_NOT_EQUAL       )) { __operator = Comparator::NotEqual;
      } else if (CurrentToken (TOKEN_LESS            )) { __operator = Comparator::Less;
      } else if (CurrentToken (TOKEN_LESS_OR_EQUAL   )) { __operator = Comparator::LessOrEqual;
      } else if (CurrentToken (TOKEN_GREATER         )) { __operator = Comparator::More ;
      } else if (CurrentToken (TOKEN_GREATER_OR_EQUAL)) { __operator = Comparator::MoreOrEqual;
      } else {
        throw UnexpectedTokenException ();
      }
      std::string __right = ReadStringLiteral ();

      request->addCondition (__operator, __left, __right);
    }
    
    AcceptToken (TOKEN_BRACKET_RIGHT);
    
    return request;
    
  } catch (...) {
    if (request != (RequestLook *) 0) delete request;
    return (Message *) 0;
  }
}

/**********************************************************************************************
 *
 * ReadOK
 *
 *********************************************************************************************/
Message * StreamReader::ReadOK () {
  AcceptToken (TOKEN_OK);
  ResponseOK * response = new ResponseOK (socket->getRemoteAddress (),
                                          socket->getRemotePort ());
  if (response == (ResponseOK *) 0) return (Message *) 0;
  return response;
}

/**********************************************************************************************
 *
 * ReadPeers
 *
 * Peers = PEERS LITERAL_STRING '[' { IP_ADDRESS ':' LITERAL_NUMERIC } ']' ;
 *
 *********************************************************************************************/ 
Message * StreamReader::ReadPeers () {
  ResponsePeers * response = new ResponsePeers (socket->getRemoteAddress (),
                                                socket->getRemotePort ());
  if (response == (ResponsePeers *) 0)
    return (Message *) 0;

  try {
    AcceptToken (TOKEN_PEERS);
    response->setKey (ReadStringLiteral ());

    AcceptToken (TOKEN_BRACKET_LEFT);    

    while (CurrentToken (TOKEN_LITERAL_STRING)) {
      std::string address = ReadStringLiteral ();
      AcceptToken (TOKEN_COLON);      
      long port = ReadNumericLiteral ();
      response->addPeer (address, port);
    }    

    AcceptToken (TOKEN_BRACKET_RIGHT);

    return response;
    
  } catch (...) {
    if (response != (ResponsePeers *) 0) delete response;
    return (Message *) 0;
  }
}

/**********************************************************************************************
 *
 * ReadUpdate
 *
 * Update = UPDATE SEED  '[' { LITERAL_STRING } ']'
 *                 LEECH '[' { LITERAL_STRING } ']' ;
 *
 *********************************************************************************************/
Message * StreamReader::ReadUpdate () {
  RequestUpdate * request = new RequestUpdate (socket->getRemoteAddress (),
                                               socket->getRemotePort ());
  if (request == (RequestUpdate *) 0)
    return (Message *) 0;

  try {
    AcceptToken (TOKEN_UPDATE);
    AcceptToken (TOKEN_SEED);
    AcceptToken (TOKEN_BRACKET_LEFT);
    
    while (CurrentToken (TOKEN_LITERAL_STRING)) {
      request->addSeed (ReadStringLiteral ());   
    }
    
    AcceptToken (TOKEN_BRACKET_RIGHT);
    
    AcceptToken (TOKEN_LEECH);
    AcceptToken (TOKEN_BRACKET_LEFT);
    
    while (CurrentToken (TOKEN_LITERAL_STRING)) {
      request->addLeech (ReadStringLiteral ());
    }
    
    AcceptToken (TOKEN_BRACKET_RIGHT);
    
    return request;
    
  } catch (...) {
    if (request != (RequestUpdate *) 0) delete request;
    return (Message *) 0;
  }
}

/**********************************************************************************************
 * SkipToken
 *********************************************************************************************/
void StreamReader::SkipToken () {
  if (length == 0) {
    ReadCharacter ();
  }
  token = NextToken ();
}

/**********************************************************************************************
 * CurrentToken
 *********************************************************************************************/
bool StreamReader::CurrentToken (StreamReader::TOKEN __token) {
  if (token == TOKEN_NONE) {
    if (length == 0) {
      ReadCharacter ();
    }
    token = NextToken ();
  }
  return token == __token;
}

/**********************************************************************************************
 * AcceptToken
 *********************************************************************************************/
void StreamReader::AcceptToken (TOKEN expected) {
  if (! CurrentToken (expected)) {
    throw UnexpectedTokenException (token, expected);
  }
  if (token != TOKEN_STREAM_END) token = TOKEN_NONE;
}
    
/**********************************************************************************************
 * ReadNumericLiteral
 *********************************************************************************************/
long StreamReader::ReadNumericLiteral () {
  AcceptToken (TOKEN_LITERAL_NUMERIC);
  long value = 0;
  std::istringstream stream (std::string ((const char *) input));
  stream >> value;
  return value;
}

/**********************************************************************************************
 * ReadStringLiteral
 *********************************************************************************************/
std::string StreamReader::ReadStringLiteral () {
  AcceptToken (TOKEN_LITERAL_STRING);
  return std::string ((const char *) input);
}

/**********************************************************************************************
 * ReadLiteral
 *********************************************************************************************/
StreamReader::TOKEN StreamReader::ReadLiteral () {
  char quote = '\0';
  if (current == '\"' || current == '\'') {
    quote = current;
    InputReset ();
  }
  
  bool isnumeric = true;
  while (ReadCharacter ()) {
    if (current == quote) {
      ReadCharacter ();
      break;

    } else if (isSpecial (current)) {
      break;

    } else if ((current < '0' || '9' < current) && current != quote) {
      isnumeric = false;
    }
    InputAppend (current);
  }

  if (isnumeric) {
    return TOKEN_LITERAL_NUMERIC;
  } else {
    return TOKEN_LITERAL_STRING;
  }
}

/**********************************************************************************************
 * NextToken
 *********************************************************************************************/
StreamReader::TOKEN StreamReader::NextToken () {
  if (token == TOKEN_STREAM_END) return TOKEN_STREAM_END;
  InputReset ();

  state = STATE_INITIAL;
  do {
    InputAppend (current);
    switch (state) {
      /* Keywords */
      /* announce */
      case STATE_A:
        if (current == 'n' || current == 'N') state = STATE_AN;
        else return ReadLiteral ();
        break;

      case STATE_AN:
        if (current == 'n' || current == 'N') state = STATE_ANN;
        else return ReadLiteral ();
        break;

      case STATE_ANN:
        if (current == 'o' || current == 'O') state = STATE_ANNO;
        else return ReadLiteral ();
        break;

      case STATE_ANNO:
        if (current == 'u' || current == 'U') state = STATE_ANNOU;
        else return ReadLiteral ();
        break;

      case STATE_ANNOU:
        if (current == 'n' || current == 'N') state = STATE_ANNOUN;
        else return ReadLiteral ();
        break;

      case STATE_ANNOUN:
        if (current == 'c' || current == 'C') state = STATE_ANNOUNC;
        else return ReadLiteral ();
        break;

      case STATE_ANNOUNC:
        if (current == 'e' || current == 'E') state = STATE_ANNOUNCE;
        else return ReadLiteral ();
        break;

      case STATE_ANNOUNCE:
        if (isSpecial (current)) return TOKEN_ANNOUNCE;
        else return ReadLiteral ();
        break;

      /* data */
      case STATE_D:
        if (current == 'a' || current == 'a') state = STATE_DA;
        else return ReadLiteral ();
        break;

      case STATE_DA:
        if (current == 't' || current == 't') state = STATE_DAT;
        else return ReadLiteral ();
        break;

      case STATE_DAT:
        if (current == 'a' || current == 'a') state = STATE_DATA;
        else return ReadLiteral ();
        break;

      case STATE_DATA:
        if (isSpecial (current)) return TOKEN_DATA;
        else return ReadLiteral ();
        break;

      /* getfile */
      case STATE_G:
        if (current == 'e' || current == 'E') state = STATE_GE;
        else return ReadLiteral ();
        break;

      case STATE_GE:
        if (current == 't' || current == 'T') state = STATE_GET;
        else return ReadLiteral ();
        break;

      case STATE_GET:
         if (current == 'f' || current == 'F') state = STATE_GETF;
        else if (current == 'p' || current == 'P') state = STATE_GETP;
        else return ReadLiteral ();
        break;

      case STATE_GETF:
        if (current == 'i' || current == 'I') state = STATE_GETFI;
        else return ReadLiteral ();
        break;

      case STATE_GETFI:
        if (current == 'l' || current == 'L') state = STATE_GETFIL;
        else return ReadLiteral ();
        break;

      case STATE_GETFIL:
        if (current == 'e' || current == 'E') state = STATE_GETFILE;
        else return ReadLiteral ();
        break;

      case STATE_GETFILE:
        if (isSpecial (current)) return TOKEN_GETFILE;
        else return ReadLiteral ();
        break;
          
      /* getpieces */
      case STATE_GETP:
        if (current == 'i' || current == 'I') state = STATE_GETPI;
        else return ReadLiteral ();
        break;

      case STATE_GETPI:
        if (current == 'e' || current == 'E') state = STATE_GETPIE;
        else return ReadLiteral ();
        break;

      case STATE_GETPIE:
        if (current == 'c' || current == 'C') state = STATE_GETPIEC;
        else return ReadLiteral ();
        break;

      case STATE_GETPIEC:
        if (current == 'e' || current == 'E') state = STATE_GETPIECE;
        else return ReadLiteral ();
        break;

      case STATE_GETPIECE:
        if (current == 's' || current == 'S') state = STATE_GETPIECES;
        else return ReadLiteral ();
        break;

      case STATE_GETPIECES:
        if (isSpecial (current)) return TOKEN_GETPIECES;
        else return ReadLiteral ();
        break;

      /* have */
      case STATE_H:
        if (current == 'a' || current == 'A') state = STATE_HA;
        else return ReadLiteral ();
        break;

      case STATE_HA:
        if (current == 'v' || current == 'V') state = STATE_HAV;
        else return ReadLiteral ();
        break;

      case STATE_HAV:
        if (current == 'e' || current == 'E') state = STATE_HAVE;
        else return ReadLiteral ();
        break;

      case STATE_HAVE:
        if (isSpecial (current)) return TOKEN_HAVE;
        else return ReadLiteral ();
        break;

      /* interested */
      case STATE_I:
        if (current == 'n' || current == 'N') state = STATE_IN;
        else return ReadLiteral ();
        break;

      case STATE_IN:
        if (current == 't' || current == 'T') state = STATE_INT;
        else return ReadLiteral ();
        break;

      case STATE_INT:
        if (current == 'e' || current == 'E') state = STATE_INTE;
        else return ReadLiteral ();
        break;

      case STATE_INTE:
        if (current == 'r' || current == 'R') state = STATE_INTER;
        else return ReadLiteral ();
        break;

      case STATE_INTER:
        if (current == 'e' || current == 'E') state = STATE_INTERE;
        else return ReadLiteral ();
        break;

      case STATE_INTERE:
        if (current == 's' || current == 'S') state = STATE_INTERES;
        else return ReadLiteral ();
        break;

      case STATE_INTERES:
        if (current == 't' || current == 'T') state = STATE_INTEREST;
        else return ReadLiteral ();
        break;

      case STATE_INTEREST:
        if (current == 'e' || current == 'E') state = STATE_INTERESTE;
        else return ReadLiteral ();
        break;

      case STATE_INTERESTE:
        if (current == 'd' || current == 'D') state = STATE_INTERESTED;
        else return ReadLiteral ();
        break;

      case STATE_INTERESTED:
        if (isSpecial (current)) return TOKEN_INTERESTED;
        else return ReadLiteral ();
        break;

      /* leech */
      case STATE_L:
             if (current == 'e' || current == 'E') state = STATE_LE;
        else if (current == 'i' || current == 'I') state = STATE_LI;
        else if (current == 'o' || current == 'O') state = STATE_LO;
        else return ReadLiteral ();
        break;

      case STATE_LE:
        if (current == 'e' || current == 'E') state = STATE_LEE;
        else return ReadLiteral ();
        break;

      case STATE_LEE:
        if (current == 'c' || current == 'C') state = STATE_LEEC;
        else return ReadLiteral ();
        break;

      case STATE_LEEC:
        if (current == 'h' || current == 'H') state = STATE_LEECH;
        else return ReadLiteral ();
        break;

      case STATE_LEECH:
        if (isSpecial (current)) return TOKEN_LEECH;
        else return ReadLiteral ();
        break;

      /* listen */
      case STATE_LI:
        if (current == 's' || current == 'S') state = STATE_LIS;
        else return ReadLiteral ();
        break;

      case STATE_LIS:
        if (current == 't' || current == 'T') state = STATE_LIST;
        else return ReadLiteral ();
        break;

      case STATE_LIST:
        if (current == 'e' || current == 'E') state = STATE_LISTE;
        else if (isSpecial (current)) return TOKEN_LIST;
        else return ReadLiteral ();
        break;

      case STATE_LISTE:
        if (current == 'n' || current == 'N') state = STATE_LISTEN;
        else return ReadLiteral ();
        break;

      case STATE_LISTEN:
            if (isSpecial (current)) return TOKEN_LISTEN;
        else return ReadLiteral ();
        break;

      /* look */
      case STATE_LO:
        if (current == 'o' || current == 'O') state = STATE_LOO;
        else return ReadLiteral ();
        break;

      case STATE_LOO:
        if (current == 'k' || current == 'K') state = STATE_LOOK;
        else return ReadLiteral ();
        break;

      case STATE_LOOK:
            if (isSpecial (current)) return TOKEN_LOOK;
        else return ReadLiteral ();
        break;

      /* ok */
      case STATE_O:
        if (current == 'k' || current == 'K') state = STATE_OK;
        else return ReadLiteral ();
        break;

      case STATE_OK:
            if (isSpecial (current)) return TOKEN_OK;
        else return ReadLiteral ();
        break;

      /* peers */
      case STATE_P:
        if (current == 'e' || current == 'E') state = STATE_PE;
        else return ReadLiteral ();
        break;

      case STATE_PE:
        if (current == 'e' || current == 'E') state = STATE_PEE;
        else return ReadLiteral ();
        break;

      case STATE_PEE:
        if (current == 'r' || current == 'R') state = STATE_PEER;
        else return ReadLiteral ();
        break;

      case STATE_PEER:
        if (current == 's' || current == 'S') state = STATE_PEERS;
        else return ReadLiteral ();
        break;

      case STATE_PEERS:
            if (isSpecial (current)) return TOKEN_PEERS;
        else return ReadLiteral ();
        break;

      /* seed */
      case STATE_S:
        if (current == 'e' || current == 'E') state = STATE_SE;
        else return ReadLiteral ();
        break;

      case STATE_SE:
        if (current == 'e' || current == 'E') state = STATE_SEE;
        else return ReadLiteral ();
        break;

      case STATE_SEE:
        if (current == 'd' || current == 'D') state = STATE_SEED;
        else return ReadLiteral ();
        break;

      case STATE_SEED:
        if (isSpecial (current)) return TOKEN_SEED;
        else return ReadLiteral ();
        break;
          
      /* update */
      case STATE_U:
        if (current == 'p' || current == 'P') state = STATE_UP;
        else return ReadLiteral ();
        break;

      case STATE_UP:
        if (current == 'd' || current == 'D') state = STATE_UPD;
        else return ReadLiteral ();
        break;

      case STATE_UPD:
        if (current == 'a' || current == 'A') state = STATE_UPDA;
        else return ReadLiteral ();
        break;

      case STATE_UPDA:
        if (current == 't' || current == 'T') state = STATE_UPDAT;
        else return ReadLiteral ();
        break;

      case STATE_UPDAT:
        if (current == 'e' || current == 'E') state = STATE_UPDATE;
        else return ReadLiteral ();
        break;

      case STATE_UPDATE:
            if (isSpecial (current)) return TOKEN_UPDATE;
        else return ReadLiteral ();
        break;
        
      case STATE_LESS:
        if (current == '=') return TOKEN_LESS_OR_EQUAL;
        else if (current == '>') return TOKEN_NOT_EQUAL;
        else return TOKEN_LESS;
        break;

      case STATE_GREATER:
        if (current == '=') return TOKEN_GREATER_OR_EQUAL;
        else return TOKEN_GREATER;
        break;
    
      default:
        // clearing input buffer content ...

        switch (current) {
          /* Keywords */
          case 'a' : case 'A' : state = STATE_A; break;
          case 'd' : case 'D' : state = STATE_D; break;
          case 'g' : case 'G' : state = STATE_G; break;
          case 'h' : case 'H' : state = STATE_H; break;
          case 'i' : case 'I' : state = STATE_I; break;
          case 'l' : case 'L' : state = STATE_L; break;
          case 'o' : case 'O' : state = STATE_O; break;
          case 'p' : case 'P' : state = STATE_P; break;
          case 's' : case 'S' : state = STATE_S; break;
          case 'u' : case 'U' : state = STATE_U; break;

          /* Separators */
          case ':' : ReadCharacter (); return TOKEN_COLON;
          case '[' : ReadCharacter (); return TOKEN_BRACKET_LEFT;
          case ']' : ReadCharacter (); return TOKEN_BRACKET_RIGHT;

          /* Operators */
          case '=' : return TOKEN_EQUAL;    break;
          case '<' : state = STATE_LESS;    break;
          case '>' : state = STATE_GREATER; break;

          /* Whitespaces */
          case ' ' : case '\t':
          case '\r': case '\n':
            InputReset ();
            break;
            
          // TODO ...
          case '\'' : case '\"' :
          case '0'  : case '1'  :
          case '2'  : case '3'  :
          case '4'  : case '5'  :
          case '6'  : case '7'  :
          case '8'  : case '9'  :
          case 'b'  : case 'B'  :
          case 'c'  : case 'C'  :
          case 'e'  : case 'E'  :
          case 'f'  : case 'F'  :
          case 'j'  : case 'J'  :
          case 'k'  : case 'K'  :
          case 'm'  : case 'M'  :
          case 'n'  : case 'N'  :
          case 'q'  : case 'Q'  :
          case 'r'  : case 'R'  :
          case 't'  : case 'T'  :
          case 'v'  : case 'V'  :
          case 'w'  : case 'W'  :
          case 'x'  : case 'X'  :
          case 'y'  : case 'Y'  :
          case 'z'  : case 'Z'  :
            return ReadLiteral ();
            
          default :
            break;
        }
        break;
    }

  } while (ReadCharacter ());
  
  /* Stream end */
  return TOKEN_STREAM_END;
}

/**********************************************************************************************
 * ReadCharacter
 *********************************************************************************************/
bool StreamReader::ReadCharacter () {
  static const int validinputs [] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0
  };
  
  bool invalid = false;
  current  = '\0';
  while (true) {
    if (&(buffer [length - 1]) < ++position) {
      if (invalid) {        
        throw InvalidInputException ();
      } else {
        long received = socket->getReceived ();
        if (!socket->Read ((uint8_t *) buffer, STREAM_BUFFER_SIZE)) {
          return false;
        }
        length = socket->getReceived () - received;
        buffer [length] = '\0';
        position = buffer;
      }
    }
    
    if (0 <= *position && *position < 128 && validinputs [*position] == 1) {
      if (! invalid) {
        current = *position;
        return true;
      } else {
        throw InvalidInputException ();
      }

    } else {
      invalid = true;
      continue;
    }
  }
  return false;
}

/**********************************************************************************************
 * ReadBinaryData
 *********************************************************************************************/
bool StreamReader::ReadBinaryData (uint8_t * data, int size) {
  int available = length - (int) (position - buffer);
  if (0 <= size && size < available) {
    memcpy (data, position, size);
    position = (char *) ((unsigned long) position + (unsigned long) size);
    return true;

  } else if (0 < available) {
    bzero (data, size);
    memcpy (data, position, available);
    if (! socket->Read ((uint8_t *) ((unsigned long) data + (unsigned long) available), size - available)) {
      return false;
    } else {
      position = buffer;
      return true;
    }
  } else {
    return false;
  }
}

/**********************************************************************************************
 * isSpecial
 *********************************************************************************************/
bool StreamReader::isSpecial (char current) {
  static const int special [] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  };
  int index = (int) current;
  return (0 <= index && index < 128 && special [index] == 1);
}

/**********************************************************************************************
 * InputReset
 *********************************************************************************************/
void StreamReader::InputReset () {
  bzero ((void *) input, STREAM_BUFFER_SIZE);
  input_position = 0;
}

/**********************************************************************************************
 * InputAppend
 *********************************************************************************************/
void StreamReader::InputAppend (char c) {
  if (input_position < STREAM_BUFFER_SIZE) {
    input [input_position++] = c;
  }
}

}  // namespace SAYL

