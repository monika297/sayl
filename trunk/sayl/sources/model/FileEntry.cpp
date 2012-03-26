/** Header **/

#include "model/FileEntry.h"

#include <string.h>

namespace SAYL {

/**********************************************************************************************
 * Constructor
 *********************************************************************************************/
FileEntry::FileEntry (File & __file, Peer __peer)
  : seed (false), leech (false),
    file (__file), peer (__peer),
    buffermap ((uint8_t*) 0)    
{
  buffermap = new uint8_t [file.getBufferMapSize ()];
  bzero ((void *) buffermap, file.getBufferMapSize ());
}
    
/**********************************************************************************************
 * Destructor
 *********************************************************************************************/
FileEntry::~FileEntry () {
  if (buffermap != (uint8_t*) 0) {
    delete [] buffermap;
  }
}
  
/**********************************************************************************************
 * getPeer
 *********************************************************************************************/
Peer FileEntry::getPeer () const {
  return peer;
}
    
/**********************************************************************************************
 * getBufferMap
 *********************************************************************************************/
const uint8_t * FileEntry::getBufferMap () const {
  return buffermap;
}

/**********************************************************************************************
 * setBufferMap
 *********************************************************************************************/
bool FileEntry::setBufferMap (uint8_t * __buffermap) {
  if (buffermap == (uint8_t*) 0)
    return false;
    
  memcpy ((void *) buffermap, (void *) __buffermap, file.getBufferMapSize ());
  return true;
}


bool FileEntry::getPiece(int index, uint8_t* piece) {
	return file.getPiece(index,piece);
}

bool FileEntry::setPiece(int index, uint8_t* piece) {
	return file.setPiece(index,piece);
}

long FileEntry::getPieceSize() const {
	return file.getPieceSize();
}

int FileEntry::getBufferMapSize() const {
	return file.getBufferMapSize();
}
std::string FileEntry::getKey()	const{
		  return file.getKey();
	  }

} // namespace SAYL

