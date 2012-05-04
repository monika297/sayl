/** Header **/

#ifndef __SAYL_MESSAGE_H__
#define __SAYL_MESSAGE_H__

#include <string>

namespace SAYL {

/**********************************************************************************************
 * Message
 *********************************************************************************************/
class Message {
public :
  /** Constructor **/
  Message (std::string __address, int __port)
    : remoteaddress (__address), remoteport (__port) {}
  virtual ~Message(){};
  /** getCommand **/
  virtual int getCommand () const = 0;
  
  /** getCommandName **/
  virtual std::string getCommandName () const=0;
  
  /** getRemoteAddress **/
  std::string getRemoteAddress () const {
    return remoteaddress;
  }
  
  /** getRemotePort **/
  int getRemotePort () const {
    return remoteport;
  }

private:
  std::string remoteaddress;
  int remoteport;

}; // class Message

}  // namespace SAYL

#endif

