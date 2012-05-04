/** Header **/

#ifndef __SAYL_FILE_ENTRY_H__
#define __SAYL_FILE_ENTRY_H__

#include <string>
#include <inttypes.h>

#include "model/File.h"
#include "model/Peer.h"

namespace SAYL {

class File;

/**********************************************************************************************
 * FileEntry
 *********************************************************************************************/
class FileEntry {
public :
  /** Constructor **/
  FileEntry (File &, Peer);
    
  /** Destructor **/
  virtual ~FileEntry ();
  
  /** getPeer **/
  Peer getPeer () const;
    
  /** getBufferMap **/
  const uint8_t * getBufferMap () const;

  /** setBufferMap **/
  bool setBufferMap (uint8_t *);
  
  /** Comparaison Operator **/
  bool operator == (const FileEntry & entry) const {
    return entry.peer == peer;
  }
  
  bool operator != (const FileEntry & entry) const {
    return entry.peer != peer;
  }
  std::string getKey() const;
  /** getPiece **/
   bool getPiece (int, uint8_t *);

   /** setPiece **/
   bool setPiece (int, uint8_t *);
   /**getPieceSize*/
   long getPieceSize () const;
   /** BufferMapSize **/
   int getBufferMapSize () const;
  /** peer set as seeder **/
  bool seed;

  /** peer set as leecher **/
  bool leech;

private :
  /* file */
  File & file;
    
  /* peer */
  Peer peer;
  
  /* buffermap */
  uint8_t * buffermap;
    
}; // class FileInstance

}  // namespace SAYL

#endif

