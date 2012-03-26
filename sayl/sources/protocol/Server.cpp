#include "Server.h"
#include "stream/StreamWriter.h"
#include "stream/StreamReader.h"
#include "services/Tracker.h"
namespace SAYL {

Server::Server() {
	// TODO Auto-generated constructor stub

}

Server::~Server() {
	// TODO Auto-generated destructor stub
}
Message* handler(Request* m){
	Tracker tracker;
	switch(m->getCommand()){
	case REQUEST_ANNOUNCE:{
		ResponseOK* r=new ResponseOK(tracker.handleAnnounce(*(RequestAnnounce*)m));
		return r;
		break;}
	case REQUEST_GETFILE:{
		ResponsePeers* r=new ResponsePeers(tracker.handleGetFile(*(RequestGetFile*)m));
		return r;
		break;}
	case REQUEST_LOOK:{
		ResponseList* r=new ResponseList(tracker.handleLook(*(RequestLook*)m));
		return r;
		break;}
	case REQUEST_UPDATE:{
		ResponseOK* r=new ResponseOK(tracker.handleUpdate(*(RequestUpdate*)m));
		return r;
		break;}
	default:
		break;
	}
	return (Response*)0;
}

bool Server::Run() {
	if (! ServerSocket::Run ()) return true;
	int connectionindex = AcceptConnection ();
	ConnectionSocket* connection;
	for (int index = 0; index < getConnectionsCount(); index ++) {
		if (0 <= connectionindex &&
				connection->isConnected()){
			connection=getConnection(index);
			StreamReader reader(connection);
			Request* m =(Request *)reader.ReadMessage();
			StreamWriter writer(connection);
			writer.Write(*handler(m));
		}
	}
}

} /* namespace SAYL */
