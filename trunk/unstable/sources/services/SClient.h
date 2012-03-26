
#ifndef SCLIENT_H_
#define SCLIENT_H_
#include "model/FileEntry.h"
#include "ClientSocket.h"
#include "Peer.h"
#include "protocol/Client.h"
#include "model/Directory.h"

namespace SAYL {

class SClient: public SAYL::ClientSocket, public SAYL::Peer ,public SAYL::Client {
public:
	SClient(long localport=0,std::string addr="");
	virtual ~SClient();
	void handler(Message& m);
private:
	Directory::clientDir directory;
};

} /* namespace SAYL */
#endif /* SCLIENT_H_ */
