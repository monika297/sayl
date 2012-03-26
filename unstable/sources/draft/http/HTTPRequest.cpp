/** Header **/
#include <stdlib.h>
#include <string.h>
#include <string>

#include "HTTPRequest.h"

namespace SAYL {

/**********************************************************************************************
 * Constructeur
 *********************************************************************************************/
HTTPRequest::HTTPRequest (ConnectionSocket* connection) :
  socket (connection),
  state (REQUEST_DONE),
  position (buffer),
  remaining (HTTP_MAX_HEADER_SIZE - 1),
  contentremaining (HTTP_MAX_CONTENT_SIZE - 1),
  method ((char*) "GET"),
  uri ((char*) ""),
  protocol ((char*) "HTTP/1.1"),
  protocolremaining ((char*) "TTP/1.0"),
  header ((char*) ""),
  value ((char*) "")
{
}

/**********************************************************************************************
 * Desctructor
 *********************************************************************************************/ 
HTTPRequest::~HTTPRequest () {}

/**********************************************************************************************
 * ReadHeaders
 *********************************************************************************************/ 
bool HTTPRequest::ReadHeaders () {
  if (socket == 0 || ! socket->isConnected ()) {
    return false;
  }

  if (state == REQUEST_ERROR || state == REQUEST_DONE) {
    // Reinitializing parser
    state = REQUEST_LINE_START;
    position = buffer;
    remaining = HTTP_MAX_HEADER_SIZE - 1;
    method = (char*) "GET";
    uri = (char*) "";
    protocol = (char*) "HTTP/1.1";
    protocolremaining = (char*) "TTP/1.0";
    header = (char*) "";
    value = (char*) "";
    bzero (buffer, HTTP_MAX_HEADER_SIZE);
    bzero (content, HTTP_MAX_CONTENT_SIZE);
  } 

  if (remaining > 0) {
    if (! socket->Read ((uint8_t*) position, remaining)) return false;
  }

  if (REQUEST_LINE_START <= state && state < REQUEST_LINE_DONE) {
    if (! ParseRequestLine ()) {
      state = REQUEST_ERROR;
      return false;
    }
  }
  
  if (state == REQUEST_LINE_DONE && protocol[7] == '9') {
    // HTTP/0.9 : skipping headers and content
    state = REQUEST_DONE;
  }
  
  if (REQUEST_LINE_DONE <= state && state <= HEADER_LINE_DONE) {  
    do {
      state = HEADER_LINE_START;
      if (!ParseHeaderLine ()) state = REQUEST_ERROR;
    } while (state == HEADER_LINE_DONE);
  }
  
  if (state == REQUEST_CONTENT) {  
    if (hasHeader ("Content-Length")) {
      contentlength = atoi (getHeader ("Content-Length").data ());
      if (contentlength < 0 || HTTP_MAX_CONTENT_SIZE < contentlength) {
        return false;
      }

    } else {
      contentlength = 0;
      contentremaining = 0;
      state = REQUEST_DONE;
    }
  }

  return (state == REQUEST_DONE || state == REQUEST_CONTENT);
}

/**********************************************************************************************
 * ReadContent
 *********************************************************************************************/ 
bool HTTPRequest::ReadContent () {
  if (socket == 0 || ! socket->isConnected ()) {
    return false;
  }

  if (state != REQUEST_CONTENT) return false;
  if (socket->Read ((uint8_t*) content, contentlength)) {
    state = REQUEST_DONE;
    return true;
  } else {
    return false;
  }
}

/**********************************************************************************************
 * getMethod
 *********************************************************************************************/ 
std::string HTTPRequest::getMethod () const {
  return method;
}

/**********************************************************************************************
 * getProtocol
 *********************************************************************************************/ 
std::string HTTPRequest::getProtocol () const {
  return protocol;
}
  
/**********************************************************************************************
 * getURI
 *********************************************************************************************/ 
std::string HTTPRequest::getURI () const {
  return uri;
}
  
/**********************************************************************************************
 * Header
 *********************************************************************************************/ 
bool HTTPRequest::hasHeader (std::string header) {
  std::map<std::string, std::string>::const_iterator iterator = headers.find (header);
  if (iterator != headers.end () && iterator->second != "") {
    return true;
  } else {
    return false;
  }
}

std::string HTTPRequest::getHeader (std::string header) {
  return headers [header];
}

bool HTTPRequest::setHeader (std::string header, std::string value) {
  headers [header] = value;
  return true;
}
  
/**********************************************************************************************
 * getData
 *********************************************************************************************/ 
std::string HTTPRequest::getContent () const {
  return content;
}

/**********************************************************************************************
 * ParseRequestLine
 *********************************************************************************************/ 
bool HTTPRequest::ParseRequestLine () {
  char * trim = 0;
  char current;

  while (*position != 0 && remaining > 0) {  
    current = *position;
    switch (state) {
      case REQUEST_LINE_START:
        if (current == '\r' || current == '\n' || current == ' ' || current == '\t') {
          break;
        } else if (current < 'A' || current > 'Z') {
          return false;
        } else {
          state = REQUEST_LINE_METHOD;
          method = position;
        }
        break;

      case REQUEST_LINE_METHOD:
        if (current == ' ' || current == '\t') {
          trim = position;
          state = REQUEST_LINE_BEFORE_URI;
        } else if (current == '\n' || current == '\r') {
          state = REQUEST_LINE_PROTOCOL;
        } else if (current < 'A' || current > 'Z') {
          return false;
        }
        break;

      case REQUEST_LINE_BEFORE_URI:
        if (trim != 0) *trim = '\0';
        if (current == '\n' || current == '\r') {
          state = REQUEST_LINE_PROTOCOL;
        } else if (current != ' ' && current != '\t') {
          uri = position;
          state = REQUEST_LINE_URI;
        }
        break;

      case REQUEST_LINE_URI:
        if (current == '\r' || current == '\n') {
          trim = position;          
          state = REQUEST_LINE_PROTOCOL;
        } else if (current == ' ' || current == '\t') {
          trim = position;
          state = REQUEST_LINE_AFTER_URI;
        } else if (! validValueCharacter (current)) {
          return false;
        }
        break;

      case REQUEST_LINE_AFTER_URI:
        if (current == '\r' || current == '\n') {
          state = REQUEST_LINE_PROTOCOL;
        } else if (current == ' ' || current == '\t') {
          break;
        } else if (current == 'H') {
          state = REQUEST_LINE_PROTOCOL;
          protocol = position;
        } else if (validValueCharacter (current)) {
          state = REQUEST_LINE_URI;
        } else {
          return false;
        }
        break;

      case REQUEST_LINE_PROTOCOL:
        if (trim != 0) *trim = '\0';
        if (!ParseProtocol ()) return false;
        break;

      case REQUEST_LINE_AFTER_PROTOCOL:
               if (current == '\r') { state = REQUEST_LINE_END;
        } else if (current == '\n') { state = REQUEST_LINE_DONE;
        } else if (current == ' ' || current == '\t') {
        } else {
          return false;
        }
        break;

      case REQUEST_LINE_END:
        if (current == '\n') {
          state = REQUEST_LINE_DONE;
        } else {
          return false;
        }
        break;

      case REQUEST_LINE_DONE:
        position ++;
        remaining --;
        return true;
    }
    if (*position != '\n') {
      remaining --;
      position ++;
    }
  }
  return true;
}

/**********************************************************************************************
 * ParseProtocol
 *********************************************************************************************/ 
bool HTTPRequest::ParseProtocol () {
  // Ignoring white spaces
  while (*protocolremaining == 'T' && (*position == ' ' || *position == '\t')) {
    position++;
  }

  // Assuming HTTP/0.9 if end of line reached
  if (*protocolremaining == 'T' && (*position == '\r' || *position == '\n')) {
    protocol = (char*) "HTTP/0.9";
    state = REQUEST_LINE_AFTER_PROTOCOL;
    return true;
  } else {
    // Comparing position with "TTP/1.x"
    while (*protocolremaining != '\0') {
      if ((*protocolremaining == '0' && *position == '1') || *position == *protocolremaining) {
        position++;
        protocolremaining++;
      } else {
        break;
      }
    }

    if (*protocolremaining == '\0') {
      *position = '\0';
      state = REQUEST_LINE_AFTER_PROTOCOL;
      return true;      
    } else if (*position == '\0') {
      // Incomplete request, waiting for next call
      return true;
    } else {
      // Input does not match
      return false;
    }
  }
}

/**********************************************************************************************
 * ParseHeaderLine
 *********************************************************************************************/ 
bool HTTPRequest::ParseHeaderLine () {
  char current;
  char * trim = 0;

  while (*position != 0 && remaining > 0) {
    current = *position;
    switch (state) {
      case HEADER_LINE_START:
        header = (char*) "";
        value = (char*) "";
        if (current == '\n') {
          state = HEADER_LINE_DONE;
        } else if (current == '\r') {
          state = HEADER_LINE_END;
        } else if (validHeaderCharacter (current)) {
          state = HEADER_LINE_NAME;
          header = position;
        } else {
          return false;
        }
        break;

      case HEADER_LINE_NAME:
        if (current == ':') {          
          trim = position;
          state = HEADER_LINE_BEFORE_VALUE;
        } else if (validHeaderCharacter (current)) {
        } else {
          return false;
        }
        break;

      case HEADER_LINE_BEFORE_VALUE:
        if (trim != 0) *trim = '\0';
        if (current == '\n') {
          state = HEADER_LINE_DONE;
        } else if (current == '\r') {
          state = HEADER_LINE_END;
        } else if (current == ' ' || current == '\t') {
        } else if (validValueCharacter(current)) {
          value = position;
          trim = 0;
          state = HEADER_LINE_VALUE;
        } else {
          return false;
        }
        break;

      case HEADER_LINE_VALUE:
        if (current == '\n') {
          state = HEADER_LINE_DONE;
        } else if (current == '\r') {
          state = HEADER_LINE_END;
        } else if (current == ' ' || current == '\t') {
          trim = position;
        } else if (! validValueCharacter(current)) {
          return false;
        }
        break;

      case HEADER_LINE_END:
        if (current == '\n') {
          state = HEADER_LINE_DONE;
        } else {
          return false;
        }
        break;

      case HEADER_LINE_DONE:        
        if (trim != 0) *trim = '\0';
        else *position = '\0';
        if (*header == '\0') { // final empty header line
          state = REQUEST_CONTENT;
        } else {
          headers [std::string (header)] = std::string (value);
        }
        position ++;
        remaining --;
        return true;
    }

    if (*position != '\n') {
      remaining --;
      position ++;
    }
  }
  return true;
}

/**********************************************************************************************
 * ParseContent
 *********************************************************************************************/ 
bool HTTPRequest::ParseContent () {
  return true;
}

}  // namespace SAYL

