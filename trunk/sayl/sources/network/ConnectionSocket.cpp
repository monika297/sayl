
#include "ConnectionSocket.h"
using namespace SAYL;
ConnectionSocket::~ConnectionSocket() {
}

ConnectionSocket::ConnectionSocket(Service& service, int descriptor):Socket(descriptor),Service(service) {
}

double ConnectionSocket::getElapsedTime() const {
	return Service::getElapsedTime();
}


