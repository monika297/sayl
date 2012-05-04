/** Header **/

namespace SAYL {
namespace XML {

/**********************************************************************************************
 * XMLScanner
 *********************************************************************************************/
class XMLScanner {
public :
  /** Constructor **/
  XMLScanner ();
  
  /** Scan **/
  void Scan (InputStream &);

private :
  /* Current element output stream */
  MemoryOutputStream content;

}; // class XMLScanner

}  // namespace XML
}  // namespace SAYL

