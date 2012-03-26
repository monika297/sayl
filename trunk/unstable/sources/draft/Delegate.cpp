/** Header **/

#include <iostream>
#include <sstream>

#include "Delegate.h"

namespace SAYL {

/**********************************************************************************************
 * SAYLGenericDelegate<bool>
 *********************************************************************************************/
template<> const std::string SAYLGenericDelegate<bool>::getTypeName (void) const {
  return "boolean";
}

template<> bool SAYLGenericDelegate<bool>::getDefaultValue (void) const {
  return false;
}

template<> const std::string SAYLGenericDelegate<bool>::getString (int index) const {
  if (getValue (index)) return "true";
  else return "false";
}
  
template<> bool SAYLGenericDelegate<bool>::setString (std::string& content, int index) {
  std::istringstream stream (content);
  bool value = false;
  stream >> value; 
  return setValue (value, index);
}

/**********************************************************************************************
 * SAYLGenericDelegate<int>
 *********************************************************************************************/
template<> const std::string SAYLGenericDelegate<int>::getTypeName (void) const {
  return "integer";
}

template<> int SAYLGenericDelegate<int>::getDefaultValue (void) const {
  return 0;
}

template<> const std::string SAYLGenericDelegate<int>::getString (int index) const {
  std::ostringstream stream;
  stream << getValue (index);
  return stream.str ();
}
  
template<> bool SAYLGenericDelegate<int>::setString (std::string& content, int index) {
  std::istringstream stream (content);
  int value = 0;
  stream >> value;
  return setValue (value, index);
}

/**********************************************************************************************
 * SAYLGenericDelegate<long>
 *********************************************************************************************/
template<> const std::string SAYLGenericDelegate<long>::getTypeName (void) const {
  return "long";
}

template<> long SAYLGenericDelegate<long>::getDefaultValue (void) const {
  return 0l;
}

template<> const std::string SAYLGenericDelegate<long>::getString (int index) const {
  std::ostringstream stream;
  stream << getValue (index);
  return stream.str ();
}
  
template<> bool SAYLGenericDelegate<long>::setString (std::string& content, int index) {
  std::istringstream stream (content);
  long value = 0l;
  stream >> value; 
  return setValue (value, index);
}

/**********************************************************************************************
 * SAYLGenericDelegate<float>
 *********************************************************************************************/
template<> const std::string SAYLGenericDelegate<float>::getTypeName (void) const {
  return "float";
}

template<> float SAYLGenericDelegate<float>::getDefaultValue (void) const {
  return 0.f;
}

template<> const std::string SAYLGenericDelegate<float>::getString (int index) const {
  std::ostringstream stream;
  stream << getValue (index);
  return stream.str ();
}
  
template<> bool SAYLGenericDelegate<float>::setString (std::string& content, int index) {
  std::istringstream stream (content);
  float value = 0.f;
  stream >> value; 
  return setValue (value, index);
}

/**********************************************************************************************
 * SAYLGenericDelegate<double>
 *********************************************************************************************/
template<> const std::string SAYLGenericDelegate<double>::getTypeName (void) const {
  return "double";
}

template<> double SAYLGenericDelegate<double>::getDefaultValue (void) const {
  return 0.;
}

template<> const std::string SAYLGenericDelegate<double>::getString (int index) const {
  std::ostringstream stream;
  stream << getValue (index);
  return stream.str ();
}
  
template<> bool SAYLGenericDelegate<double>::setString (std::string& content, int index) {
  std::istringstream stream (content);
  double value = 0.;
  stream >> value; 
  return setValue (value, index);
}

/**********************************************************************************************
 * SAYLGenericDelegate<std::string>
 *********************************************************************************************/
template<> const std::string SAYLGenericDelegate<std::string>::getTypeName (void) const {
  return "string";
}

template<> std::string SAYLGenericDelegate<std::string>::getDefaultValue (void) const {
  return "";
}

template<> const std::string SAYLGenericDelegate<std::string>::getString (int index) const {
  std::ostringstream stream;
  stream << getValue (index);
  return stream.str ();
}
  
template<> bool SAYLGenericDelegate<std::string>::setString (std::string& content, int index) {
  std::istringstream stream (content);
  std::string value;
  stream >> value; 
  return setValue (value, index);
}

/**********************************************************************************************
 * SAYLGenericDelegate<Model*>
 *********************************************************************************************/
template<> const std::string SAYLGenericDelegate<Model*>::getTypeName (void) const {
  return "object";
}

template<> Model* SAYLGenericDelegate<Model*>::getDefaultValue (void) const {
  return 0;
}

template<> const std::string SAYLGenericDelegate<Model*>::getString (int index) const {
  std::ostringstream stream;
  stream.setf ( std::ios::hex, std::ios::basefield );
  stream << "object@" << (unsigned long) getValue (index);
  return stream.str ();
}
  
template<> bool SAYLGenericDelegate<Model*>::setString (std::string& content, int index) {
  return false;
}

}  // namespace SAYL

