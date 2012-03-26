/** Header **/

#include <string.h>

#include "messages/Request.h"

namespace SAYL {


/** Announce **/

/**********************************************************************************************
 * getPort
 *********************************************************************************************/
int RequestAnnounce::getPort () const {
  return port;
}

/**********************************************************************************************
 * setPort
 *********************************************************************************************/
void RequestAnnounce::setPort (int __port) {
  port = __port;
}
  
/**********************************************************************************************
 * getSeedCount
 *********************************************************************************************/
int RequestAnnounce::getSeedCount () const {
  return seed.size ();
}
  
/**********************************************************************************************
 * getSeedFileName
 *********************************************************************************************/
std::string RequestAnnounce::getSeedFileName (int index) const {
  return seed.at (index).filename;
}
  
/**********************************************************************************************
 * getSeedKey
 *********************************************************************************************/
std::string RequestAnnounce::getSeedKey (int index) const {
  return seed.at (index).key;
}
  
/**********************************************************************************************
 * getSeedFileSize
 *********************************************************************************************/
long RequestAnnounce::getSeedFileSize (int index) const {
  return seed.at (index).filesize;
}
  
/**********************************************************************************************
 * getSeedPieceSize
 *********************************************************************************************/
long RequestAnnounce::getSeedPieceSize (int index) const {
  return seed.at (index).piecesize;
}
  
/**********************************************************************************************
 * addSeed
 *********************************************************************************************/
void RequestAnnounce::addSeed (std::string filename, std::string key, long size, long piecesize) {
  FileInfo file;
  file.filename = filename;
  file.key = key;
  file.filesize = size;
  file.piecesize = piecesize;
  seed.push_back (file);
}
  
/**********************************************************************************************
 * getLeechCount
 *********************************************************************************************/
int RequestAnnounce::getLeechCount () const {
  return leech.size ();
}
  
/**********************************************************************************************
 * getLeech
 *********************************************************************************************/
std::string RequestAnnounce::getLeechKey (int index) const {
  return leech.at(index);
}
  
/**********************************************************************************************
 * addLeech
 *********************************************************************************************/
void RequestAnnounce::addLeech (std::string key) {
  leech.push_back (key);
}

/** GetFile **/


/**********************************************************************************************
 * getKey
 *********************************************************************************************/
std::string RequestGetFile::getKey () const {
  return key;
}
  
/**********************************************************************************************
 * setKey
 *********************************************************************************************/
void RequestGetFile::setKey (std::string __key) {
  key = __key;
}


/** GetPieces **/


/**********************************************************************************************
 * getKey
 *********************************************************************************************/
std::string RequestGetPieces::getKey () const {
  return key;
}
  
/**********************************************************************************************
 * setKey
 *********************************************************************************************/
void RequestGetPieces::setKey (std::string __key) {
  key = __key;
}

/**********************************************************************************************
 * getPieceIndexCount
 *********************************************************************************************/
int RequestGetPieces::getPieceIndexCount () const {
  return pieces.size ();
}
  
/**********************************************************************************************
 * getPiece
 *********************************************************************************************/
int RequestGetPieces::getPieceIndex (int index) const {
  return pieces.at (index);
}
  
/**********************************************************************************************
 * addPiece
 *********************************************************************************************/
void RequestGetPieces::addPieceIndex (int piece) {
  pieces.push_back (piece);
}


/** Have **/


/**********************************************************************************************
 * Constructor
 *********************************************************************************************/
RequestHave::RequestHave (std::string __address, int __port, int __size)
  : Request (__address, __port), size (0), buffermap ((uint8_t *) 0)
{
  if (__size > 0) {
    size = __size;
    buffermap = new uint8_t [size];
    bzero ((void *) buffermap, size);
  }
}

/**********************************************************************************************
 * Destructor
 *********************************************************************************************/
RequestHave::~RequestHave () {
  if (buffermap != (uint8_t *) 0) {
    delete [] buffermap;
  }
}

/**********************************************************************************************
 * getKey
 *********************************************************************************************/
std::string RequestHave::getKey () const {
  return key;
}
  
/**********************************************************************************************
 * setKey
 *********************************************************************************************/
void RequestHave::setKey (std::string __key) {
  key = __key;
}

/**********************************************************************************************
 * getBufferMapSize
 *********************************************************************************************/
int RequestHave::getBufferMapSize () const {
  return size;
}

/**********************************************************************************************
 * getBufferMap
 *********************************************************************************************/
const uint8_t * RequestHave::getBufferMap () const {
  return buffermap;
}
  
/**********************************************************************************************
 * setBufferMap
 *********************************************************************************************/
bool RequestHave::setBufferMap (uint8_t * buffer) {
  if (buffermap != (uint8_t *) 0) {
    memcpy (buffermap, buffer, size);
    return true;
  } else {
    return false;
  }
}


/** Interested **/


/**********************************************************************************************
 * getKey
 *********************************************************************************************/
std::string RequestInterested::getKey () const {
  return key;
}
  
/**********************************************************************************************
 * setKey
 *********************************************************************************************/
void RequestInterested::setKey (std::string __key) {
  key = __key;
}


/** Look **/


/**********************************************************************************************
 * getConditionsCount
 *********************************************************************************************/
int RequestLook::getConditionsCount () const {
  return conditions.size ();
}
/**********************************************************************************************
 * conditionCheckFile
 **********************************************************************************************/
bool  RequestLook::conditionCheckFile (int index,File & file) {
  return conditions.at (index).look(file);
}

/**********************************************************************************************
 * getConditionOperator
 *********************************************************************************************/
Comparator::Operator RequestLook::getConditionOperator (int index) const {
  return conditions.at (index).getOperator();
}

/**********************************************************************************************
 * getConditionLeftOperand
 *********************************************************************************************/
std::string RequestLook::getConditionLeftOperand (int index) const {
  return conditions.at (index).getAttribute();
}

/**********************************************************************************************
 * getConditionRightOperand
 *********************************************************************************************/
std::string RequestLook::getConditionRightOperand (int index) const {
  return conditions.at (index).getValue();
}
  
/**********************************************************************************************
 * addCondition
 *********************************************************************************************/
void RequestLook::addCondition (Comparator::Operator __operator,
                                std::string __left, std::string __right)
{
  Criterion condition(__left,__operator,__right);

  conditions.push_back (condition);
}


/** Update **/


/**********************************************************************************************
 * getSeedCount
 *********************************************************************************************/
int RequestUpdate::getSeedCount () const {
  return seed.size ();
}
  
/**********************************************************************************************
 * getSeed
 *********************************************************************************************/
std::string RequestUpdate::getSeed (int index) const {
  return seed.at (index);
}
  
/**********************************************************************************************
 * addSeed
 *********************************************************************************************/
void RequestUpdate::addSeed (std::string key) {
  seed.push_back (key);
}
  
/**********************************************************************************************
 * getLeechCount
 *********************************************************************************************/
int RequestUpdate::getLeechCount () const {
  return leech.size ();
}
  
/**********************************************************************************************
 * getLeech
 *********************************************************************************************/
std::string RequestUpdate::getLeech (int index) const {
  return leech.at (index);
}
  
/**********************************************************************************************
 * addLeech
 *********************************************************************************************/
void RequestUpdate::addLeech (std::string key) {
  leech.push_back (key);
}

} // namespace SAYL

