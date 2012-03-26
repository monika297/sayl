/** Header **/

namespace SAYL {
namespace XML {

/**********************************************************************************************
 * Parser
 *********************************************************************************************/
class Parser {
public :
  /** Constructor **/
  Parser ();
  
  /** Parse **/
  void Parse (InputStream &);

  /** setContentHandler **/
  ContentHandler & getContentHandler () const;
  void setContentHandler (ContentHandler &);

  /** setErrorHandler **/
  ErrorHandler & getErrorHandler () const;
  void setErrorHandler (ErrorHandler &);

private :
  /* Content handler */
  ContentHandler * contenthandler;
  
  /* Error handler */
  ErrorHandler * errorhandler;
  
  /* Current element output stream */
  MemoryOutputStream content;

}; // class Parser

}  // namespace XML
}  // namespace SAYL

