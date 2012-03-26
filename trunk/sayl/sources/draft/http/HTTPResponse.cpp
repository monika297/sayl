/** Header **/
#include <string>
#include <iostream>
#include <sstream>

#include "HTTPResponse.h"

namespace SAYL {

/**********************************************************************************************
 * Constructeur
 *********************************************************************************************/ 
HTTPResponse::HTTPResponse (ConnectionSocket* connection) : socket (connection) {
  int status = 200;
  protocol = "HTTP/1.1";
}

/**********************************************************************************************
 * Desctructor
 *********************************************************************************************/ 
HTTPResponse::~HTTPResponse () {}

/**********************************************************************************************
 * Write
 *********************************************************************************************/ 
bool HTTPResponse::Write () {
  if (socket == 0 || ! socket->isConnected ()) {
    return false;
  }

  std::ostringstream stream;
  if (protocol == "HTTP/0.9") {
    stream << content;

  } else {
    stream << protocol << " " << status << " " << getStatusDescription () << "\r\n" ;
    std::map <std::string, std::string>::iterator header = headers.begin ();
    while (header != headers.end ()) {
      stream << header->first << ": " << header->second << "\r\n" ;
      header ++;
    }
    stream << "\r\n" << content;
  }
  std::string content = stream.str ();
  return (socket->Write ((uint8_t*)content.data (), content.length ()));
}
  
/**********************************************************************************************
 * Status
 *********************************************************************************************/ 
int HTTPResponse::getStatus () const {
  return status;
}

bool HTTPResponse::setStatus (int code) {
  if (100 <= code && code < 600) {
    status = code;
    return true;
  } else {
    return false;
  }
}

std::string HTTPResponse::getStatusDescription () const {
  switch (status) {
    case 100 : return "Continue";
    case 101 : return "Switching Protocols";
    case 200 : return "OK";
    case 201 : return "Created";
    case 202 : return "Accepted";
    case 203 : return "Non-Authoritative Information";
    case 204 : return "No Content";
    case 205 : return "Reset Content";
    case 206 : return "Partial Content";
    case 300 : return "Multiple Choices";
    case 301 : return "Moved Permanently";
    case 302 : return "Found";
    case 303 : return "See Other";
    case 304 : return "Not Modified";
    case 305 : return "Use Proxy";
    case 307 : return "Temporary Redirect";
    case 400 : return "Bad Request";
    case 401 : return "Unauthorized";
    case 402 : return "Payment Required";
    case 403 : return "Forbidden";
    case 404 : return "Not Found";
    case 405 : return "Method Not Allowed";
    case 406 : return "Not Acceptable";
    case 407 : return "Proxy Authentication Required";
    case 408 : return "Request Time-out";
    case 409 : return "Conflict";
    case 410 : return "Gone";
    case 411 : return "Length Required";
    case 412 : return "Precondition Failed";
    case 413 : return "Request Entity Too Large";
    case 414 : return "Request-URI Too Large";
    case 415 : return "Unsupported Media Type";
    case 416 : return "Requested range not satisfiable";
    case 417 : return "Expectation Failed";
    case 500 : return "Internal Server Error";
    case 501 : return "Not Implemented";
    case 502 : return "Bad Gateway";
    case 503 : return "Service Unavailable";
    case 504 : return "Gateway Time-out";
    case 505 : return "HTTP Version not supported";
    default  : return "";
  }
}

/**********************************************************************************************
 * getProtocol
 *********************************************************************************************/ 
std::string HTTPResponse::getProtocol () const {
  return protocol;
}

bool HTTPResponse::setProtocol (std::string version) {
  if (version == "HTTP/1.1" || version == "HTTP/1.0" || version == "HTTP/0.9") {
    protocol = version;
  } else {
    return false;
  }
}

/**********************************************************************************************
 * Headers
 *********************************************************************************************/ 
std::string HTTPResponse::getHeader (std::string header) {
  if (headers.count (header)) return headers [header];
  else return "";
}

bool HTTPResponse::setHeader (std::string header, std::string value) {
  if (validHeader (header) && validHeaderValue (value)) {
    headers [header] = value;
    return true;
  } else {
    return false;
  }
}

/**********************************************************************************************
 * Content
 *********************************************************************************************/ 
std::string HTTPResponse::getContent () const {
  return content;
}

bool HTTPResponse::setContent (std::string text) {
  content = text;
}

/**********************************************************************************************
 * validHeader
 *********************************************************************************************/ 
bool HTTPResponse::validHeader (std::string& header) const {
  for (unsigned int index = 0; index < header.length (); index++) {
    if (('a' <= header [index] && header [index] <= 'z') || (header [index] == '-') ||
        ('A' <= header [index] && header [index] <= 'Z') ) continue;
    else return false;
  }
  return true;
}

/**********************************************************************************************
 * validHeaderValue
 *********************************************************************************************/ 
bool HTTPResponse::validHeaderValue (std::string& value) const {
  for (unsigned int index = 0; index < value.length (); index++) {
    if ((char) 31 < value[index] && value[index] < (char) 127) continue;
    else return false;
  }
  return true;
}

}  // namespace SAYL

