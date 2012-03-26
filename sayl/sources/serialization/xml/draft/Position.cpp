/** Header **/

#include "Position.h"

namespace SAYL {
namespace XML {

/**********************************************************************************************
 * Constructor
 *********************************************************************************************/
Position::Position () : line (0), column (0) {}
  
Position::Position (int __line, int __column) {
  setLine (__line);
  setColumn (__column);
}

/**********************************************************************************************
 * Line
 *********************************************************************************************/
void Position::setLine (int __line) const {
  if (0 <= __line) {
    line = __line;
  } else {
    throw IllegalArgumentException ();
  }
}

int Position::getLine () const {
  return line;
}

/**********************************************************************************************
 * Column
 *********************************************************************************************/
void Position::setColumn (int __column) const {
  if (0 <= __column) {
    column = __column;
  } else {
    throw IllegalArgumentException ();
  }
}

int Position::getColumn () const {
  return column;
}

}  // namespace XML
}  // namespace SAYL

