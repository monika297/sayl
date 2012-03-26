/** Header **/

namespace SAYL {
namespace XML {

/**********************************************************************************************
 * Position
 *********************************************************************************************/
class Position {
public :
  /** Constructor **/
  Position ();  
  Position (int, int);

  /** Line **/
  void setLine (int) const;
  int getLine () const;

  /** Column **/
  void setColumn (int);
  int getColumn () const;
  
private :
  int line;
  int column;

}; // class Position

}  // namespace XML
}  // namespace SAYL

