#include "Tracker.h"
#include "stream/StreamWriter.h"
#include "messages/Response.h"
namespace SAYL{
SAYL::Tracker::Tracker() {
}

SAYL::Tracker::~Tracker() {
}
ResponseOK SAYL::Tracker::handleAnnounce(RequestAnnounce& requestAnnouce) {
	File file;int i;ResponseOK resp(requestAnnouce.getRemoteAddress(),requestAnnouce.getRemotePort());
	for(i=0;i<requestAnnouce.getSeedCount();i++){
		file=File(requestAnnouce.getSeedFileName(i),requestAnnouce.getSeedKey(i)
				,requestAnnouce.getSeedFileSize(i),requestAnnouce.getSeedPieceSize(i));
		file=files.insert(file);
		file.addSeeder(Peer(requestAnnouce.getRemoteAddress()
				,requestAnnouce.getRemotePort()));
	}
	for(i=0;i<requestAnnouce.getLeechCount();i++){
		file=files.getFile(requestAnnouce.getLeechKey(i));
		file.addLeecher(Peer(requestAnnouce.getRemoteAddress(),requestAnnouce.getRemotePort()));
	}
	return (resp);
}

ResponsePeers SAYL::Tracker::handleGetFile(RequestGetFile& requestGetFile) {
	ResponsePeers response(requestGetFile.getRemoteAddress()
			,requestGetFile.getRemotePort());
	std::string key=requestGetFile.getKey();
	response.setKey(key);
	File file=files.getFile(key);
	File::PeerIterator peer= file.getPeers();
	while(!peer.End()){
		if(peer.isSeeder()){
			response.addPeer(peer.getAddress(),peer.getPort());
		}
		peer.Next();
	}
	return (response);
}

ResponseList SAYL::Tracker::handleLook(RequestLook& requestLook) {
	ResponseList response(requestLook.getRemoteAddress()
			,requestLook.getRemotePort());
	Directory<File>::iterator it;
	File file;
	for(int i;i<requestLook.getConditionsCount();i++){
		GoThroughDir(it,files){//FixMe conditions need to be multiplied not added
			file =it->second;
			if(requestLook.conditionCheckFile(i,file)){
				response.addFile(file.getFilename(),file.getKey()
						,file.getFileSize(),file.getPieceSize());
			}
		}
	}
	return (response);
}

ResponseOK SAYL::Tracker::handleUpdate(RequestUpdate& requestUpdate) {
	File file;int i;ResponseOK resp(requestUpdate.getRemoteAddress()
				,requestUpdate.getRemotePort());
	for(i=0;i<requestUpdate.getSeedCount();i++){
		file=files.getFile(requestUpdate.getSeed(i));
		file.addSeeder(Peer(requestUpdate.getRemoteAddress()
				,requestUpdate.getRemotePort()));
	}
	for(i=0;i<requestUpdate.getLeechCount();i++){
		file=files.getFile(requestUpdate.getLeech(i));
		file.addLeecher(Peer(requestUpdate.getRemoteAddress()
				,requestUpdate.getRemotePort()));
	}
	return (resp);
}


}
