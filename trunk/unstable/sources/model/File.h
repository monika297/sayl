/** Header **/

#ifndef __SAYL_FILE_H__
#define __SAYL_FILE_H__

#include <string>
#include <map>

#include "model/FileEntry.h"
#include "model/Peer.h"

namespace SAYL {

class FileEntry;

/**********************************************************************************************
 * File
 *********************************************************************************************/
class File {
public :
  typedef std::map <Peer, FileEntry *> EntrySet;
  typedef std::map <Peer, FileEntry *>::const_iterator EntrySetIterator;
  
  /********************************************************************************************
   *  PeerIterator
   *******************************************************************************************/
  class PeerIterator {
  protected :
    friend class File;

    /** Constructor **/
    PeerIterator (EntrySetIterator start, EntrySetIterator end) : inner (start), limit (end) {}
    
  public :
    /** Destructor **/
    ~PeerIterator () {}
;
    /** Next **/
    bool Next ();
    
    /** End **/
    bool End () const;
    
    /** getAddress **/
    std::string getAddress () const;

    /** getPort **/
    int getPort () const;
    
    /** isSeeder **/
    bool isSeeder () const;
    
    /** isLeecher **/
    bool isLeecher () const;
    
    /** getBufferMap **/
    const uint8_t * getBufferMap () const;

    /** setBufferMap **/
    bool setBufferMap (uint8_t * content);
  
  private :  
    EntrySetIterator inner;
    EntrySetIterator limit;
    
  }; // class PeerIterator

  /** Constructor **/
  File (const std::string, const std::string, long, long);
  File():filename(""),key(""),size(0),piecesize(0){}
  /** Destructor **/
  virtual ~File ();

  /** getFilename **/
  std::string getFilename () const;

  /**getLocal*/
  const uint8_t* getLocalBufferMap() ;
  /** getKey **/
  std::string getKey () const;

  /** getFileSize **/
  long getFileSize () const;
  
  /** BufferMapSize **/
  int getBufferMapSize () const;


  /* LocalFile */

  /** isLocalFile **/
  bool isLocalFile () const;

  /** getLocalFileName **/
  std::string getLocalFileName () const;

  /** setLocalFileName **/
  bool setLocalFileName (std::string);


  /* Pieces */

  /** getPieceSize **/
  long getPieceSize () const;
  
  /** getPiecesCount **/
  int getPiecesCount () const;

  /** getPiece **/
  bool getPiece (int, uint8_t *);

  /** setPiece **/
  bool setPiece (int, uint8_t *);


  /* Peers */

  /** getLeechers **/
  PeerIterator getPeers () const;

  /** getLeechersCount **/
  int getLeechersCount () const;
  
  /** addLeecher **/
  void addLeecher (Peer);
  
  /** removeLeecher **/
  void removeLeecher (Peer);
  
  /** getSeedersCount **/
  int getSeedersCount () const;
  
  /** addSeeder **/
  void addSeeder (Peer);
  
  /** removeSeeder **/
  void removeSeeder (Peer);

  /** getPeerBufferMap **/
  const uint8_t * getPeerBufferMap (Peer);

  /** setPeerBufferMap **/
  bool setPeerBufferMap (Peer, uint8_t *);

  /** Comparison Operator **/
  bool operator == (const File &) const;
  bool operator != (const File &) const;

  bool operator <  (const File &) const;
  bool operator <= (const File &) const;

  bool operator >  (const File &) const;
  bool operator >= (const File &) const;
private :
  /** getFileEntry **/
  FileEntry * getFileEntry (Peer);

  /** removeFileEntry **/
  void removeFileEntry (Peer);

  /* File attributes */
  std::string filename;
  std::string key;
  long size;
  long piecesize;

  /* counters */
  int seeders;
  int leechers;

  /* file entries */
  EntrySet entries;

  /* local file */
  std::string localfilename;
  int descriptor;

}; // class File

}  // namespace SAYL

#endif

