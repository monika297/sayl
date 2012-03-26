/** Header **/

#include "model/File.h"
#include "network/ServerConnectionSocket.h"
#include "stream/StreamWriter.h"
#include "stream/StreamReader.h"

using namespace SAYL;

#include <iostream>

int main (int size, char** arguments) {
  /*****************************************************************************************
*/
  File file ("sample","testkey",1052, 16);
  file.setLocalFileName ("sample.txt");
  uint8_t buffer [16] = {'h', 'e', 'l', 'l', 'o', ',' ,' ', 'm', 'o', 'r', 'l', 'd', ' ', 'o', 'f', '\0'};
  file.setPiece (5, buffer);
  file.getPiece (2, buffer);
  std::cout << (const char*) buffer << std::endl;
  
  file.addSeeder (Peer(".1", 70));
  file.addSeeder (Peer(".2", 80));
  file.addSeeder (Peer(".4", 90));
  
  file.removeSeeder (Peer(".1", 70));
  
  file.addLeecher (Peer(".2", 80));
  
  file.removeSeeder (Peer(".2", 80));
  //file.removeLeecher (Peer(".2", 80));

  file.addLeecher (Peer(".4", 90));
  
  File::PeerIterator iterator = file.getPeers ();

  std::cout << (Peer(".1", 70) < Peer(".2", 80)) << " "
            << (Peer(".1", 70) < Peer(".4", 90)) << " "
            << (Peer(".2", 80) < Peer(".4", 90)) << std::endl;

  std::cout << "Seeders : " << file.getSeedersCount ()
            << " Leechers : " << file.getLeechersCount () << std::endl;
  
  do {
    std::cout << iterator.getAddress () << ":" << iterator.getPort () << "   "
              << iterator.isSeeder ()   << " " << iterator.isLeecher () << std::endl;
  } while (iterator.Next());

  /**//*****************************************************************************************
  */
	ServerSocket sock;
	ConnectionSocket socket(sock);

  StreamWriter writer (&socket);
  
  RequestAnnounce announce ("localhost", 7000);
  announce.setPort (8080);
  announce.addSeed ("file1", "key1", 1024, 128);
  announce.addSeed ("file2", "key2", 486, 32);
  announce.addLeech ("key3");
  announce.addLeech ("key4");
  writer.Write (announce);
  
  /* ... */
  
  RequestGetFile getfile ("localhost", 7000);
  getfile.setKey ("requestedkeyfile");
  writer.Write (getfile);
  
  /* ...*/
  
  RequestGetPieces getpieces ("localhost", 7000);
  getpieces.setKey ("downloadablekeyfile");
  getpieces.addPieceIndex (7);
  getpieces.addPieceIndex (4);
  getpieces.addPieceIndex (3);
  getpieces.addPieceIndex (9);
  writer.Write (getpieces);
  
 /*... */

  /*****************************************************************************************/
  std::cout << std::endl << "StreamReader" << std::endl;
  ConnectionSocket second(sock);
  StreamWriter tester (&second);
  
  StreamReader reader (&socket);
  const Message * m = reader.ReadMessage ();
  while (m != (Message *) 0) {
    tester.Write (*m);
    m = reader.ReadMessage ();
  }
  std::cout << std::endl;

  return 0;
}

