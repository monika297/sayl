/** Header **/

#include "model/File.h"

#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#include <iostream>

namespace SAYL {

/**********************************************************************************************
 * Constructor
 *********************************************************************************************/
File::File (const std::string __filename, const std::string __key,
            long __size, long __piecesize)
  : filename (__filename), key (__key),
    size (__size), piecesize (__piecesize),
    seeders (0), leechers (0),
    descriptor (-1)
{  
  // ...
}

/**********************************************************************************************
 * Destructor
 *********************************************************************************************/
File::~File () {
  if (descriptor != -1) {
    close (descriptor);
  }
  
  EntrySetIterator iterator = entries.begin ();
  while (iterator != entries.end ()) {    
    delete iterator->second;
    iterator ++;
  }  
}

/**********************************************************************************************
 * getFilename
 *********************************************************************************************/
std::string File::getFilename () const {
  return filename;
}
 
/**********************************************************************************************
 * getKey
 *********************************************************************************************/
std::string File::getKey () const {
  return key;
}

/**********************************************************************************************
 * getFileSize
 *********************************************************************************************/
long File::getFileSize () const {
  return size;
}
  
/**********************************************************************************************
 * BufferMapSize
 *********************************************************************************************/
int File::getBufferMapSize () const {
  int pieces = getPiecesCount ();
  if ((pieces % 8) == 0) return (pieces/ 8);
  else return (pieces/ 8) + 1;
}

/**********************************************************************************************
 * isLocalFile
 *********************************************************************************************/
bool File::isLocalFile () const {
  return descriptor != -1;
}

/**********************************************************************************************
 * getLocalFilename
 *********************************************************************************************/
std::string File::getLocalFileName () const {
  return localfilename;
}

/**********************************************************************************************
 * setLocalFileName
 *********************************************************************************************/
bool File::setLocalFileName (std::string local) {
  int __descriptor = open (local.c_str (), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR |
                                                             S_IRWXG | S_IRGRP);
  if (__descriptor == -1) {
    return false;

  } else {
    if (descriptor != -1) {
      close (descriptor);
    }
    descriptor = __descriptor;
    
    if (posix_fallocate (descriptor, 0, size) != 0) {
      std::cerr << "failed disk space allocation." << std::endl;
      close (descriptor);
      descriptor = -1;
      localfilename = local;
      return false;
    } else {
      localfilename = local;
      return true;
    }
  }
}

/**********************************************************************************************
 * getPieceSize
 *********************************************************************************************/
long File::getPieceSize () const {
  return piecesize;
}
  
/**********************************************************************************************
 * getPiecesCount
 *********************************************************************************************/
int File::getPiecesCount () const {
  if ((size % piecesize) == 0) return (size / piecesize);
  else return (size / piecesize) + 1;
}

/**********************************************************************************************
 * getPiece
 *********************************************************************************************/
bool File::getPiece (int index, uint8_t * content) {
  if (descriptor == -1) {
    std::cerr << "no file specified for read operation." << std::endl;
  }

  int count = getPiecesCount ();
  if (0 <= index && index < count) {
    
    if (lseek(descriptor, piecesize*index, SEEK_SET) == -1) {
      std::cerr << "error while reading content." << std::endl;
      return false;
    }
  
    int current = piecesize;
    if (index == count - 1) current = size % piecesize;

    if (read (descriptor, content, current) == -1) {
      std::cerr << "error while reading content." << std::endl;
      return false;
    } else {
      return true;
    }
    
  } else {
    std::cerr << "invalid piece index provided. (" << count << ")"  << std::endl;
    return false;
  }
}

/**********************************************************************************************
 * setPiece
 *********************************************************************************************/
bool File::setPiece (int index, uint8_t * content) {
  if (descriptor == -1) {
    std::cerr << "no file specified for read operation." << std::endl;
  }

  int count = getPiecesCount ();
  if (0 <= index && index < count) {
    
    if (lseek(descriptor, piecesize*index, SEEK_SET) == -1) {
      std::cerr << "error while reading content." << std::endl;
      return false;
    }
  
    int current = piecesize;
    if (index == count - 1) current = size % piecesize;

    if (write (descriptor, content, current) == -1) {
      std::cerr << "error while writing content." << std::endl;
      return false;
    } else {
      return true;
    }
    
  } else {
    std::cerr << "invalid piece index provided. (" << count << ")"  << std::endl;
    return false;
  }
}

/**********************************************************************************************
 * getPeers
 *********************************************************************************************/
File::PeerIterator File::getPeers () const {
  return File::PeerIterator (entries.begin (), entries.end ());
}

/**********************************************************************************************
 * getLeechersCount
 *********************************************************************************************/
int File::getLeechersCount () const {
  return leechers;
}

/**********************************************************************************************
 * addLeecher
 *********************************************************************************************/
void File::addLeecher (Peer peer) {
  FileEntry * entry = getFileEntry (peer);
  if (entry != (FileEntry*) (0)) {
		entry->leech = true;
		leechers++;
	}
}

/**********************************************************************************************
 * removeLeecher
 *********************************************************************************************/
void File::removeLeecher(Peer peer) {
	FileEntry* entry = getFileEntry(peer);
	if (entry != (FileEntry*) (0)) {
		entry->leech = false;
		leechers--;
		if (!entry->leech && !entry->seed) {
			removeFileEntry(peer);
		}
	}

}

/**********************************************************************************************
 * getSeedersCount
 *********************************************************************************************/
int File::getSeedersCount() const {
	return seeders;
}

/**********************************************************************************************
 * addSeeder
 *********************************************************************************************/
void File::addSeeder(Peer peer) {
	FileEntry* entry = getFileEntry(peer);
	if (entry != (FileEntry*) (0)) {
		entry->seed = true;
		seeders++;
	}
}

/**********************************************************************************************
 * removeSeeder
 *********************************************************************************************/
void File::removeSeeder(Peer peer) {
	FileEntry* entry = getFileEntry(peer);
	if (entry != (FileEntry*) (0)) {
		entry->seed = false;
		seeders--;
		if (!entry->leech && !entry->seed) {
			removeFileEntry(peer);
		}
	}

}

/**********************************************************************************************
 * getPeerBufferMap
 *********************************************************************************************/
const uint8_t* File::getPeerBufferMap(Peer peer) {
	FileEntry* entry = getFileEntry(peer);
	if (entry != (FileEntry*) (0)) {
		return entry->getBufferMap();
	}
	return (uint8_t*) (0);
}

/**********************************************************************************************
 * setPeerBufferMap
 *********************************************************************************************/
bool File::setPeerBufferMap(Peer peer, uint8_t* map) {
	FileEntry* entry = getFileEntry(peer);
	if (entry != (FileEntry*) (0)) {
		return entry->setBufferMap(map);
	}
	return false;
}

/**********************************************************************************************
 * getFileEntry
 *********************************************************************************************/
FileEntry* File::getFileEntry(Peer peer) {
	if (entries.count(peer) == 1) {
		return entries[peer];
	} else if (entries.count(peer) == 0) {
		FileEntry* entry = new FileEntry(*this, peer);
		if (entry == (FileEntry*) (0)) {
			std::cerr << "Memory allocation failed." << std::endl;
		}
		entries[peer] = entry;
		return entries[peer];
	} else {
		return (FileEntry*) (0);
	}
}

/**********************************************************************************************
 * removeFileEntry
 *********************************************************************************************/
void File::removeFileEntry(Peer peer) {
	entries.erase(peer);
}

/**********************************************************************************************
 * 
 * File::PeerIterator
 *
 *********************************************************************************************/
/**********************************************************************************************
 * Next
 *********************************************************************************************/
bool File::PeerIterator::Next() {
	inner++;
	return inner != limit;
}

/**********************************************************************************************
 * End
 *********************************************************************************************/
bool File::PeerIterator::End() const {
	return inner == limit;
}

/**********************************************************************************************
 * getAddress
 *********************************************************************************************/
std::string File::PeerIterator::getAddress() const {
	return inner->first.getAddress();
}

/**********************************************************************************************
 * getPort
 *********************************************************************************************/
int File::PeerIterator::getPort() const {
	return inner->first.getPort();
}

/**********************************************************************************************
 * isSeeder
 *********************************************************************************************/
bool File::PeerIterator::isSeeder() const {
	return inner->second->seed;
}

/**********************************************************************************************
 * isLeecher
 *********************************************************************************************/
bool File::PeerIterator::isLeecher() const {
	return inner->second->leech;
}

/**********************************************************************************************
 * getBufferMap
 *********************************************************************************************/
const uint8_t* File::PeerIterator::getBufferMap() const {
	return inner->second->getBufferMap();
}

const uint8_t* File::getLocalBufferMap() {
	return getPeers().inner->second->getBufferMap();
}

/**********************************************************************************************
 * setBufferMap
 *********************************************************************************************/
bool File::PeerIterator::setBufferMap (uint8_t * content) {
  return inner->second->setBufferMap (content);
}
/**************************************************************************//**
 * Comparison Operator
 *****************************************************************************/
bool File::operator ==(const File& file) const {
	return file.getKey()==key;
}

bool File::operator !=(const File& file) const {
	return file.getKey()!=key;
}

bool File::operator <(const File& file) const {
	return file.getFileSize()<size;
}

bool File::operator <=(const File& file) const {
	return file.getFileSize()<=size;
}

bool File::operator >(const File& file) const {
	return file.getFileSize()>size;
}

bool File::operator >=(const File& file) const {
	return file.getFileSize()>=size;
}

} // namespace SAYL

