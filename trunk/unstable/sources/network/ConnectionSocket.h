#ifndef CONNECTIONSOCKET_H_
#define CONNECTIONSOCKET_H_

#include "Service.h"
#include "Socket.h"
#include <iostream>
namespace SAYL{
class ConnectionSocket: public Service, public Socket {
public:
	ConnectionSocket(Service& ,int descriptor=0);
	virtual ~ConnectionSocket();
	double getElapsedTime() const;
};
}//namespace
#endif /* CONNECTIONSOCKET_H_ */
