/** Header **/
#ifndef __SAYL_STREAM_READER_H__
#define __SAYL_STREAM_READER_H__

#include <inttypes.h> /* Integer types definitions */

#include "network/Socket.h"

#include "messages/Message.h"
#include "messages/Request.h"
#include "messages/Response.h"

#define STREAM_BUFFER_SIZE 10240

namespace SAYL {

/**********************************************************************************************
 * StreamReader
 *********************************************************************************************/
class StreamReader {
public :
  /** Constructor **/
  StreamReader (Socket*);
  
  /** Destructor **/
  ~StreamReader ();

  /** ReadMessage **/ 
  const Message * ReadMessage (int partsize = 0);

private :
  /********************************************************************************************
   * TOKEN
   *******************************************************************************************/
  enum TOKEN {
    TOKEN_NONE,
    TOKEN_INVALID,
    TOKEN_STREAM_END,
    TOKEN_COLON,
    TOKEN_EQUAL,
    TOKEN_NOT_EQUAL, // 5
    TOKEN_LESS,
    TOKEN_LESS_OR_EQUAL,
    TOKEN_GREATER,
    TOKEN_GREATER_OR_EQUAL,
    TOKEN_ANNOUNCE, // 10
    TOKEN_DATA,
    TOKEN_GETFILE,
    TOKEN_GETPIECES,
    TOKEN_HAVE,
    TOKEN_INTERESTED, // 15
    TOKEN_LIST,
    TOKEN_LISTEN,
    TOKEN_LEECH,
    TOKEN_LOOK,
    TOKEN_PEERS, // 20
    TOKEN_OK,
    TOKEN_SEED,
    TOKEN_UPDATE,
    TOKEN_BRACKET_LEFT,
    TOKEN_BRACKET_RIGHT, // 25
    TOKEN_LITERAL_STRING,
    TOKEN_LITERAL_NUMERIC,
  }; // enum TOKEN
  
  /********************************************************************************************
   * UnexpectedTokenException
   *******************************************************************************************/
  class UnexpectedTokenException {
  public:    
    /** Constructor **/
    UnexpectedTokenException () : expected (TOKEN_NONE) {}

    /** Constructor **/
    UnexpectedTokenException (TOKEN __found, TOKEN __expected) : expected (__expected) {}
    
    /** getToken **/
    TOKEN getToken () {
      return expected;
    }
    
  private:
    TOKEN expected;
  };
  
  /********************************************************************************************
   * InvalidInputException
   *******************************************************************************************/
  class InvalidInputException {};

  /********************************************************************************************
   * STATE
   *******************************************************************************************/
  enum STATE {
    STATE_INITIAL,
    STATE_STREAM_END,
    STATE_LESS,
    STATE_GREATER,
    STATE_A, STATE_AN, STATE_ANN, STATE_ANNO, STATE_ANNOU, STATE_ANNOUN, STATE_ANNOUNC, STATE_ANNOUNCE,
    STATE_D, STATE_DA, STATE_DAT, STATE_DATA,
    STATE_G, STATE_GE, STATE_GET, STATE_GETF, STATE_GETFI, STATE_GETFIL, STATE_GETFILE,
                                  STATE_GETP, STATE_GETPI, STATE_GETPIE, STATE_GETPIEC, STATE_GETPIECE, STATE_GETPIECES,
    STATE_H, STATE_HA, STATE_HAV, STATE_HAVE,
    STATE_I, STATE_IN, STATE_INT, STATE_INTE, STATE_INTER, STATE_INTERE, STATE_INTERES, STATE_INTEREST, STATE_INTERESTE, STATE_INTERESTED,
    STATE_L, STATE_LE, STATE_LEE, STATE_LEEC, STATE_LEECH, 
             STATE_LI, STATE_LIS, STATE_LIST,
                                              STATE_LISTE, STATE_LISTEN,
             STATE_LO, STATE_LOO, STATE_LOOK,
    STATE_O, STATE_OK,
    STATE_P, STATE_PE, STATE_PEE, STATE_PEER, STATE_PEERS, 
    STATE_S, STATE_SE, STATE_SEE, STATE_SEED,
    STATE_U, STATE_UP, STATE_UPD, STATE_UPDA, STATE_UPDAT,  STATE_UPDATE,
  }; // enum STATE

  /** Input reader */
  char buffer [STREAM_BUFFER_SIZE];
  
  /** input buffer */
  char input [STREAM_BUFFER_SIZE];
  
  /** input position **/
  int input_position;

  /** current position */
  char * position;

  /** token */
  TOKEN token;

  /** parser state */
  STATE state;

  /** read characters */
  char current;

  /** read data size */
  int length;
  
  /** ConnectionSocket **/
  Socket * socket;
  
  /** CurrentMessage **/
  Message * message;
  
  /** ReadAnnounce **/
  Message * ReadAnnounce ();

  /** ReadData **/
  Message * ReadData (int);

  /** ReadGetFile **/
  Message * ReadGetFile ();

  /** ReadGetPieces **/ 
  Message * ReadGetPieces ();

  /** ReadHave **/ 
  Message * ReadHave (int);

  /** ReadInterested **/ 
  Message * ReadInterested ();

  /** ReadList **/ 
  Message * ReadList ();

  /** ReadLook **/ 
  Message * ReadLook ();

  /** ReadOK **/
  Message * ReadOK ();

  /** ReadPeers **/ 
  Message * ReadPeers ();

  /** ReadUpdate **/
  Message * ReadUpdate ();

  /** SkipToken **/
  void SkipToken ();

  /** CurrentToken **/
  bool CurrentToken (TOKEN);

  /** AcceptToken **/
  void AcceptToken (TOKEN token = TOKEN_NONE);

  /** ReadLiteral **/
  TOKEN ReadLiteral ();
      
  /** NextToken **/
  TOKEN NextToken ();

  /** ReadNumericLiteral **/
  long ReadNumericLiteral ();

  /** ReadStringLiteral **/
  std::string ReadStringLiteral ();

  /** ReadCharacter **/
  bool ReadCharacter ();

  /** ReadBinaryData **/
  bool ReadBinaryData (uint8_t *, int);
  
  /** isSpecial **/
  bool isSpecial (char);

  /** InputReset **/  
  void InputReset ();

  /** InputAppend **/
  void InputAppend (char);

}; // class StreamReader

} // namespace SAYL

#endif

