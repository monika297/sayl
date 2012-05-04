#ifndef DECODEUTF8_H_
#define DECODEUTF8_H_
#include <vector>
#include <cstdlib>
#define MASKBITS 0x3F
#define MASKBYTE 0x80
#define MASK2BYTES 0xC0
#define MASK3BYTES 0xE0
#define MASK4BYTES 0xF0
#define MASK5BYTES 0xF8
#define MASK6BYTES 0xFC
typedef unsigned short Unicode2Bytes;
typedef unsigned int Unicode4Bytes;
typedef unsigned char byte;
namespace SAYL{
/*************************************************************************************//**
 * nom de domaine rassemblant les méthodes permettant d'encoder et de decoder
 * de l'unicode à 2 ou 4 octet
 * en encodage utf8.
 */

namespace Utf8 {
void encode2BytesUnicode(std::vector< Unicode2Bytes > input,
		std::vector< byte >& output);
void decode2BytesUnicode(std::vector< byte > input,
		std::vector< Unicode2Bytes >& output);
void encode4BytesUnicode(std::vector< Unicode4Bytes > input,
		std::vector< byte >& output);
void decode4BytesUnicode(std::vector< byte > input,
		std::vector< Unicode4Bytes >& output);
}//Utf8
}//namespace SAYL


#endif /* DECODEUTF8_H_ */
