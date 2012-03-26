/** Header **/

namespace SAYL {
namespace XML {

/**********************************************************************************************
 * ContentHandler
 *********************************************************************************************/
class ContentHandler {
public :
  /** DocumentStart **/
  virtual void DocumentStart (const Element &) = 0;

  /** DocumentEnd **/
  virtual void DocumentEnd (const Element &) = 0;

  /** ElementStart **/
  virtual void ElementStart (const Element &) = 0;

  /** ElementEnd **/
  virtual void ElementEnd (const Element &) = 0;

  /** Content **/
  virtual void Content (InputStream &) = 0;
  
  /** Warning **/
  virtual void Warning (ParseException &) = 0;

  /** Error **/
  virtual void Error (ParseException &) = 0;

}; // class ContentHandler

}  // namespace XML
}  // namespace SAYL

