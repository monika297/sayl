/** Header **/

#include "Model.h"
#include <string.h>

namespace SAYL {

/**********************************************************************************************
 * Destructor
 *********************************************************************************************/
Model::~Model () {
  MapIterator iterator = elements.begin ();
  while (iterator != elements.end ()) {
    if (iterator->second != 0) {
      delete iterator->second;
    }
    iterator++;
  }
  elements.clear ();
}

/**********************************************************************************************
 * hasElement
 *********************************************************************************************/
bool Model::hasElement (const std::string name) const {
  return (getDelegate (name) != 0);
}

/**********************************************************************************************
 * getElements
 *********************************************************************************************/
Model::Iterator Model::getElements () {
  return Model::Iterator (elements.begin (), elements.end ());
}

/**********************************************************************************************
 * getTypeName
 *********************************************************************************************/
std::string Model::getTypeName (const std::string name) const {
  Delegate * element = getDelegate (name);
  if (element == 0) {
    return "";
  } else {
    return element->getTypeName ();
  }
}

/**********************************************************************************************
 * isReadOnly
 *********************************************************************************************/
bool Model::isReadOnly (const std::string name) const {
  Delegate * element = getDelegate (name);
  if (element == 0) {
    return true;
  } else {
    return element->isReadOnly ();
  }
}

/**********************************************************************************************
 * isIndexed
 *********************************************************************************************/
bool Model::isIndexed (const std::string name) const {
  Delegate * element = getDelegate (name);
  if (element == 0) {
    return false;
  } else {
    return element->isIndexed ();
  }
}

/**********************************************************************************************
 * getIndexCount
 *********************************************************************************************/
int Model::getIndexCount (const std::string name) const {
  Delegate * element = getDelegate (name);
  if (element == 0 && element->isIndexed ()) {
    return 0;
  } else {
    return element->getElementsCount ();
  }
}

/**********************************************************************************************
 * getModel
 *********************************************************************************************/
Model * Model::getModel (const std::string name, int index) const {
  Delegate * element = getDelegate (name);
  if (element == 0 || element->getTypeName () != "object") {
    return 0;    
  } else {
    return ((SAYLGenericDelegate <Model *> *) element)->getValue (index);
  }
}

/**********************************************************************************************
 * getBoolean and setBoolean
 *********************************************************************************************/
bool Model::getBoolean (const std::string name, int index) const {
  Delegate * element = getDelegate (name);  
  if (element == 0 || element->getTypeName () != "boolean") {
    return false;    
  } else {
    return ((SAYLGenericDelegate <bool> *) element)->getValue (index);
  }
}

bool Model::setBoolean (const std::string name, bool value, int index) {
  Delegate * element = getDelegate (name);
  if (element == 0 || element->getTypeName () != "boolean") {
    return false;
  } else {
    return ((SAYLGenericDelegate <bool> *) element)->setValue (value, index);
  }
}

/**********************************************************************************************
 * getInteger and setInteger
 *********************************************************************************************/
int Model::getInteger (const std::string name, int index) const {
  Delegate * element = getDelegate (name);
  if (element == 0 || element->getTypeName () != "integer") {
    return 0;    
  } else {
    return ((SAYLGenericDelegate <int> *) element)->getValue (index);
  }
}

bool Model::setInteger (const std::string name, int value, int index) {
  Delegate * element = getDelegate (name);
  if (element == 0 || element->getTypeName () != "integer") {
    return false;
  } else {
    return ((SAYLGenericDelegate <int> *) element)->setValue (value, index);
  }
}

/**********************************************************************************************
 * getLong and setLong
 *********************************************************************************************/
long Model::getLong (const std::string name, int index) const {
  Delegate * element = getDelegate (name);
  if (element == 0 || element->getTypeName () != "long") {
    return 0l;    
  } else {
    return ((SAYLGenericDelegate <long> *) element)->getValue (index);
  }
}

bool Model::setLong (const std::string name, long value, int index) {
  Delegate * element = getDelegate (name);
  if (element == 0 || element->getTypeName () != "long") {
    return false;
  } else {
    return ((SAYLGenericDelegate <long> *) element)->setValue (value, index);
  }
}

/**********************************************************************************************
 * getFloat and setFloat
 *********************************************************************************************/
float Model::getFloat (const std::string name, int index) const {
  Delegate * element = getDelegate (name);
  if (element == 0 || element->getTypeName () != "float") {
    return 0.;    
  } else {
    return ((SAYLGenericDelegate <float> *) element)->getValue (index);
  }
}

bool Model::setFloat (const std::string name, float value, int index) {
  Delegate * element = getDelegate (name);
  if (element == 0 || element->getTypeName () != "float") {
    return false;
  } else {
    return ((SAYLGenericDelegate <float> *) element)->setValue (value, index);
  }
}

/**********************************************************************************************
 * getDouble and setDouble
 *********************************************************************************************/
double Model::getDouble (const std::string name, int index) const {
  Delegate * element = getDelegate (name);
  
  MapIterator iterator = elements.begin ();
  while (iterator != elements.end ()) {
    iterator++;
  }  
  
  if (element == 0 || element->getTypeName () != "double") {
    return 0.;    
  } else {
    return ((SAYLGenericDelegate <double> *) element)->getValue (index);
  }
}

bool Model::setDouble (const std::string name, double value, int index) {
  Delegate * element = getDelegate (name);
  if (element == 0 || element->getTypeName () != "double") {
    return false;
  } else {
    return ((SAYLGenericDelegate <double> *) element)->setValue (value, index);
  }
}

/**********************************************************************************************
 * getString and setString
 *********************************************************************************************/
std::string Model::getString (const std::string name, int index) const {
  Delegate * element = getDelegate (name);
  if (element == 0) {
    return "";    
  } else {
    return element->getString (index);
  }
}

bool Model::setString (const std::string name, std::string value, int index) {
  Delegate * element = getDelegate (name);
  if (element == 0) {
    return false;
  } else {
    return element->setString (value, index);
  }
}

/**********************************************************************************************
 * Unbind
 *********************************************************************************************/
void Model::Unbind (const std::string & name) {
  UnbindDelegate (name);
}

/**********************************************************************************************
 * Bind
 *********************************************************************************************/
void Model::BindDelegate (const std::string name, Delegate * delegate) {
  if (delegate != 0) {
    if (elements [name] != 0) {
      delete elements [name];
      elements [name] = 0;
    }
    elements [name] = delegate;
  }
}

/**********************************************************************************************
 * Unbind
 *********************************************************************************************/
void Model::UnbindDelegate (const std::string name) {
  Delegate * delegate = getDelegate (name);
  if (delegate != 0) {
    if (elements [name] != 0) {
      delete elements [name];
    }
    elements.erase (name);
  }
}

/**********************************************************************************************
 * getDelegate
 *********************************************************************************************/
Delegate * Model::getDelegate (const std::string name) const {
  char * path = new char [name.length () + 1];
  memcpy (path, name.data (), name.length ());
  path [name.length ()] = '\0';
  Delegate * delegate = getDelegateModel (path);  
  delete [] path;
  return delegate;
}

/**********************************************************************************************
 * getDelegateModel
 *********************************************************************************************/
Delegate * Model::getDelegateModel (char * path) const {
  // Ignore leading slashes
  while (*path == '/') path++;
  // determinig first separator (sequence of slashes)
  char * position  = path;
  char * separator = path;
  while (*position != '\0') {
    if  (*position == '/' ) {
      if (separator == path) separator = position;
      position ++;
      
    } else {
      if (separator != path) break;
      else position ++;      
    }
  }

  if (separator != path) *separator = '\0';

  if (*position == '\0' && *path == '\0') {
    // requested path is a sequence of slashes
    return 0;
    
  } else if (*position == '\0' || *path == '\0') {  
    // requested path does not contains a sequence slashes
    // or only in the begining and/or the end.
    if (*path == '\0') path = position;
    else position = path;    
    while (*position != '\0') {
      if  (*position == '.' ) {
        *position++ = '\0';
        break;
      } else {
        position ++;
      }
    }
    // Retreiving delegate    
    Model::MapIterator iterator = elements.find (path);
    if (iterator != elements.end () && iterator->second != 0) {
      if (iterator->second->getTypeName () == "object") {
        // Iterating ...
        Model * model = ((SAYLGenericDelegate <Model *>*) iterator->second)->getValue ();
        if (model != 0) return model->getDelegateAttribute (position);
        
      } else if (*position == '\0') {
        return iterator->second;
      }
    }
    
  } else {
    // requested path contains two or more 'parts' seperated by sequences of slashes
    Model::MapIterator iterator = elements.find (path);
    if (iterator != elements.end () && iterator->second != 0) {
      if (iterator->second->getTypeName () == "object") {
        // Iterating ...
        Model * model = ((SAYLGenericDelegate <Model *>*) iterator->second)->getValue ();
        if (model != 0) return model->getDelegateModel (position);
                
      }
    }
  }
  return 0;
}

/**********************************************************************************************
 * getDelegateAttribute
 *********************************************************************************************/
Delegate * Model::getDelegateAttribute (char * name) const {
  Model::MapIterator iterator = elements.find (name); 
  if (iterator != elements.end () &&
      iterator->second != 0 &&
      iterator->second->getTypeName () != "object") return iterator->second;
  else return 0;
}

/**********************************************************************************************
 *
 * Model::Iterator
 *
 *********************************************************************************************/

/**********************************************************************************************
 * End
 *********************************************************************************************/
bool Model::Iterator::End () const {
  return inner == limit;
}

/**********************************************************************************************
 * Next
 *********************************************************************************************/
bool Model::Iterator::Next () {
  if (! End ()) inner++;
  return End ();
}

/**********************************************************************************************
 * Name
 *********************************************************************************************/
std::string Model::Iterator::getName () const {
  return inner->first;
}

/**********************************************************************************************
 * TypeName
 *********************************************************************************************/
std::string Model::Iterator::getTypeName () const {
  return inner->second->getTypeName ();
}

/**********************************************************************************************
 * getElements
 *********************************************************************************************/
Model::Iterator Model::Iterator::getElements (int index) const {
  if (inner->second->getTypeName () == "object") {
    Model * model = ((SAYLGenericDelegate <Model*> *) inner->second)->getValue (index);
    if (model != 0) {
      return model->getElements ();
    }    
  }
  return Model::Iterator (empty.end (), empty.end ());
}

/**********************************************************************************************
 * isReadOnly
 *********************************************************************************************/
bool Model::Iterator::isReadOnly () const {
  return inner->second->isReadOnly ();
}

/**********************************************************************************************
 * isIndexed
 *********************************************************************************************/
bool Model::Iterator::isIndexed () const {
  return inner->second->isIndexed ();
}

/**********************************************************************************************
 * getElementsCount
 *********************************************************************************************/
int Model::Iterator::getElementsCount () const {
  return inner->second->getElementsCount ();
}

/**********************************************************************************************
 * Model
 *********************************************************************************************/
Model * Model::Iterator::getModel (int index) const {
  if (inner->second->getTypeName () == "object") {
    return ((SAYLGenericDelegate <Model*> *) inner->second)->getValue (index);
  } else {
    return 0;
  }
}

/**********************************************************************************************
 * Boolean
 *********************************************************************************************/
bool Model::Iterator::getBoolean (int index) const {
  if (inner->second->getTypeName () == "boolean") {
    return ((SAYLGenericDelegate <bool> *) inner->second)->getValue (index);
  } else {
    return 0;
  }
}

bool Model::Iterator::setBoolean (bool value, int index) {
  if (inner->second->getTypeName () == "boolean") {
    return ((SAYLGenericDelegate <bool> *) inner->second)->setValue (value, index);
  } else {
    return 0;
  }
}

/**********************************************************************************************
 * Integer
 *********************************************************************************************/
int Model::Iterator::getInteger (int index) const {
  if (inner->second->getTypeName () == "integer") {
    return ((SAYLGenericDelegate <int> *) inner->second)->getValue (index);
  } else {
    return 0;
  }
}

bool Model::Iterator::setInteger (int value, int index) {
  if (inner->second->getTypeName () == "integer") {
    return ((SAYLGenericDelegate <int> *) inner->second)->setValue (value, index);
  } else {
    return 0;
  }
}

/**********************************************************************************************
 * Long
 *********************************************************************************************/
long Model::Iterator::getLong (int index) const {
  if (inner->second->getTypeName () == "long") {
    return ((SAYLGenericDelegate <long> *) inner->second)->getValue (index);
  } else {
    return 0;
  }
}

bool Model::Iterator::setLong (long value, int index) {
  if (inner->second->getTypeName () == "long") {
    return ((SAYLGenericDelegate <long> *) inner->second)->setValue (value, index);
  } else {
    return 0;
  }
}

/**********************************************************************************************
 * Float
 *********************************************************************************************/
float Model::Iterator::getFloat (int index) const {
  if (inner->second->getTypeName () == "float") {
    return ((SAYLGenericDelegate <float> *) inner->second)->getValue (index);
  } else {
    return 0.;
  }
}

bool Model::Iterator::setFloat (float value, int index) {
  if (inner->second->getTypeName () == "float") {
    return ((SAYLGenericDelegate <float> *) inner->second)->setValue (value, index);
  } else {
    return false;
  }
}

/**********************************************************************************************
 * Double
 *********************************************************************************************/
double Model::Iterator::getDouble (int index) const {
  if (inner->second->getTypeName () == "double") {
    return ((SAYLGenericDelegate <double> *) inner->second)->getValue (index);
  } else {
    return 0.;
  }
}

bool Model::Iterator::setDouble (double value, int index) {
  if (inner->second->getTypeName () == "double") {
    return ((SAYLGenericDelegate <double> *) inner->second)->setValue (value, index);
  } else {
    return false;
  }
}

/**********************************************************************************************
 * String
 *********************************************************************************************/
std::string Model::Iterator::getString (int index) const {
  return inner->second->getString (index);
}

bool Model::Iterator::setString (std::string value, int index) {
  return inner->second->setString (value, index);
}

}  // namespace SAYL

