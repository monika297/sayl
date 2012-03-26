/** Header **/
#ifndef __SAYL_HTTP_RESPONSE_H__
#define __SAYL_HTTP_RESPONSE_H__

#include <inttypes.h> /* Integer types definitions */

#include "Model.h"
#include "ConnectionSocket.h"

/** HTTP status codes **/
#define HTTP_CONTINUE 100
#define HTTP_SWITCHING PROTOCOLS 101
#define HTTP_OK 200
#define HTTP_CREATED 201
#define HTTP_ACCEPTED 202
#define HTTP_NON_AUTHORITATIVE_INFORMATION 203
#define HTTP_NO_CONTENT 204
#define HTTP_RESET_CONTENT 205
#define HTTP_PARTIAL_CONTENT 206
#define HTTP_MULTIPLE_CHOICES 300
#define HTTP_MOVED_PERMANENTLY 301
#define HTTP_FOUND 302
#define HTTP_SEE_OTHER 303
#define HTTP_NOT_MODIFIED 304
#define HTTP_USE_PROXY 305
#define HTTP_TEMPORARY_REDIRECT 307
#define HTTP_BAD_REQUEST 400
#define HTTP_UNAUTHORIZED 401
#define HTTP_PAYMENT_REQUIRED 402
#define HTTP_FORBIDDEN 403
#define HTTP_NOT_FOUND 404
#define HTTP_METHOD_NOT_ALLOWED 405
#define HTTP_NOT_ACCEPTABLE 406
#define HTTP_PROXY_AUTHENTICATION REQUIRED 407
#define HTTP_REQUEST_TIME_OUT 408
#define HTTP_CONFLICT 409
#define HTTP_GONE_410
#define HTTP_LENGTH_REQUIRED 411
#define HTTP_PRECONDITION_FAILED 412
#define HTTP_REQUEST_ENTITY_TOO_LARGE 413
#define HTTP_REQUEST_URI_TOO_LARGE 414
#define HTTP_UNSUPPORTED_MEDIA_TYPE 415
#define HTTP_REQUESTED_RANGE_NOT_SATISFIABLE 416
#define HTTP_EXPECTATION_FAILED 417
#define HTTP_INTERNAL_SERVER_ERROR 500
#define HTTP_NOT_IMPLEMENTED 501
#define HTTP_BAD_GATEWAY 502
#define HTTP_SERVICE_UNAVAILABLE 503
#define HTTP_GATEWAY_TIME_OUT 504
#define HTTP_VERSION_NOT_SUPPORTED 505

namespace SAYL {

/**********************************************************************************************
 * HTTPResponse
 *********************************************************************************************/
class HTTPResponse : public Model {
public :
  /** Constructor **/
  HTTPResponse (ConnectionSocket*);
  
  /** Desctructor **/
  ~HTTPResponse ();

  /** Write **/
  bool Write ();
  
  /** Status **/
  int getStatus () const;
  bool setStatus (int);

  /** StatusDescription **/
  std::string getStatusDescription () const;

  /** getProtocol **/
  std::string getProtocol () const;
  bool setProtocol (std::string);
    
  /** Headers **/
  std::string getHeader (std::string);
  bool setHeader (std::string, std::string);

  /** Content **/
  std::string getContent () const;
  bool setContent (std::string);

private :
  /** validHeader **/ 
  bool validHeader (std::string&) const;

  /** validHeaderValue **/ 
  bool validHeaderValue (std::string&) const;

  ConnectionSocket * socket;
  int status;
  std::string content;
  std::string protocol;
  std::map <std::string, std::string> headers;
    
}; // class HTTPResponse

} // namespace SAYL

#endif

