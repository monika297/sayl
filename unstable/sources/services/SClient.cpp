#include "SClient.h"
#include "inttypes.h"

namespace SAYL {

SClient::SClient(long localport,std::string addr):Peer(addr,localport),ClientSocket(),Client(){
	port=remoteport;
}

SClient::~SClient() {

}
/**handler*/
void SClient::handler(Message& m){

}
/** GetPieces **/
ResponseData SClient::GetPieces (RequestGetPieces &requestGetPieces,File& file){
	std::string key =requestGetPieces.getKey();
	int i,index;
	unsigned char* buffer;
	int port =requestGetPieces.getRemotePort();
	std::string address=requestGetPieces.getRemoteAddress();
	int pieceSize=file.getPieceSize();
	ResponseData response(address,port,pieceSize);
	for(i=0;i<requestGetPieces.getPieceIndexCount();i++){
		index=requestGetPieces.addPieceIndex(i);
		file.getPiece(index,buffer);//TODO gestion d'erreur
		response.addPiece(index,buffer);
	}
	return response;
}

/** Interested **/
ResponseHave SClient::Interested (RequestInterested &requestInterested,File& file){
	std::string key =requestInterested.getKey();
	ResponseHave response(requestInterested.getRemoteAddress()
				,requestInterested.getRemotePort(),file.getBufferMapSize());
	response.setBufferMap((uint8_t*) (file.getLocalBufferMap()));
	return response;
}

/**Have*/
ResponseHave SClient::Have(RequestHave& requestHave,File& mfile) {
	ResponseHave have(requestHave.getRemoteAddress(),
			requestHave.getRemotePort(), requestHave.getBufferMapSize());
	return have;
}

/** handleData **/
void SClient::handleData(ResponseData& responseData, File& mfile) {
	int i, index;
	FileEntry file = FileEntry(mfile, *(this));
	file = directory.insert(file);
	for (i = 0; i < responseData.getPieceCount(); i++) {
		index = responseData.getPieceIndex(i);
		file.setPiece(index, (uint8_t*) (responseData.getPieceData(index)));
	}
}
}
