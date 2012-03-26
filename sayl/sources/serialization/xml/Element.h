/** Header **/

#include <string>

namespace SAYL {
namespace XML {

/**********************************************************************************************
 * Element
 *********************************************************************************************/
class Element {
public :
  /** Constructor **/
  Element ();

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

  /** getAttribute **/
  Attribute getAttribute (int index) const;

  Attribute getAttribute (std::string & qualifiedname) const;
  
  Attribute getAttribute (std::string & uri, std::string & localname) const;
  
  /** addAttribute **/
  void addAttribute (Attribute & attribute);

private :
  /* Element uri */
  std::string elementuri;
  
  /* Element name */
  std::string elementname;

  /* Attributes indexes */
  std::map <std::string, int> attributes;

  /* Attributes values */
  std::vector <std::string> values;

}; // class Element

}  // namespace XML
}  // namespace SAYL

