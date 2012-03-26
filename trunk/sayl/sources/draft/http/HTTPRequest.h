/** Header **/
#ifndef __SAYL_HTTP_REQUEST_H__
#define __SAYL_HTTP_REQUEST_H__

#include <inttypes.h> /* Integer types definitions */

#include "Model.h"
#include "ConnectionSocket.h"

/* Maximium header size allowed */
#define HTTP_MAX_HEADER_SIZE (32*1024)
#define HTTP_MAX_CONTENT_SIZE (256*1024)

namespace SAYL {

/**********************************************************************************************
 * HTTPRequest
 *********************************************************************************************/
class HTTPRequest : public Model {
public :
  /** Constructor **/
  HTTPRequest (ConnectionSocket*);
  
  /** Desctructor **/
  ~HTTPRequest ();

  /** ReadHeaders **/
  bool ReadHeaders ();

  /** ReadContent **/
  bool ReadContent ();
  
  /** getMethod **/
  std::string getMethod () const;
  
  /** getProtocol **/
  std::string getProtocol () const;
  
  /** getURI **/
  std::string getURI () const;
  
  /** Header **/
  bool hasHeader (std::string);
  std::string getHeader (std::string);
  bool setHeader (std::string, std::string);
  
  /** getData **/
  std::string getContent () const;

private :
  /** ParseRequestLine **/
  bool ParseRequestLine ();
  
  /** ParseProtocol **/ 
  bool ParseProtocol ();

  /** ParseHeaderLine **/ 
  bool ParseHeaderLine ();

  /** ParseContent **/ 
  bool ParseContent ();
  
  /** validHeaderCharacter **/ 
  inline bool validHeaderCharacter (char value) const {
    return (('a' <= value && value <= 'z') || (value == '-') ||('A' <= value && value <= 'Z'));
  }

  /** validValueCharacter **/ 
  inline bool validValueCharacter (char value) const {
    return ((char) 31 < value && value < (char) 127);
  }

  /** ParserState **/
  enum ParserState {
    /* General */
    REQUEST_ERROR = 0,
    REQUEST_CONTENT,
    REQUEST_DONE,
    /* REQUEST_LINE */
    REQUEST_LINE_START,
    REQUEST_LINE_METHOD,
    REQUEST_LINE_BEFORE_URI,
    REQUEST_LINE_URI,
    REQUEST_LINE_AFTER_URI,
    REQUEST_LINE_PROTOCOL,
    REQUEST_LINE_AFTER_PROTOCOL,
    REQUEST_LINE_END,
    REQUEST_LINE_DONE,
    /* HEADER_LINE */
    HEADER_LINE_START,
    HEADER_LINE_NAME,
    HEADER_LINE_BEFORE_VALUE,
    HEADER_LINE_VALUE,
    HEADER_LINE_END,
    HEADER_LINE_DONE,
  };
  
  // Connection
  ConnectionSocket * socket;
  enum ParserState state;

  // Parser
  char buffer [HTTP_MAX_HEADER_SIZE];
  char content [HTTP_MAX_CONTENT_SIZE];
  int  contentlength;
  int  contentremaining;
  char * position;
  int remaining;

  // Properties
  char * method;
  char * uri;
  char * protocol;
  char * protocolremaining;
  char * header;
  char * value;
  std::map <std::string, std::string> headers;
  
}; // class HTTPRequest

} // namespace SAYL

#endif

