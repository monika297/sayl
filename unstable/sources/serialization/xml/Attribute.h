/** Header **/

#include <string>

namespace SAYL {
namespace XML {

/**********************************************************************************************
 * Attribute
 *********************************************************************************************/
class Attribute {
public :
  /** Constructor **/
  Attribute ();

  /** Prefix **/
  std::string getPrefix () const;

  void setPrefix (std::string);
 
  /** LocalName **/
  std::string getLocalName () const;

  void setLocalName (std::string);

  /** QualifiedName **/
  std::string getQualifiedName () const;

  void setQualifiedName (std::string);

  /** URI **/
  std::string getURI () const;

  void setURI (std::string);

  /** Value **/
  std::string getValue () const;
  
  void setValue (std::string) const;

private :
  /* URI */
  std::string uri;
  
  /* Prefix */
  std::string prefix;
  
  /* Local name */
  std::string local;
  
  /* Value */
  std::string local;

}; // class Attribute

}  // namespace XML
}  // namespace SAYL

