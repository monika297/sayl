#ifndef __SAYL_DELEGATE_H__
#define __SAYL_DELEGATE_H__

#include <string>

namespace SAYL {

class Model;

/********************************************************************************************//**
 * Delegate
 *********************************************************************************************/
class Delegate {
public :
  /** getTypeName **/
  virtual const std::string getTypeName (void) const = 0;

  /** isReadOnly **/
  virtual bool isReadOnly (void) const = 0;

  /** isIndexed **/
  virtual bool isIndexed (void) const = 0;

  /** getElementsCount **/
  virtual int getElementsCount (void) const = 0;
  
  /** getString **/
  virtual const std::string getString (int index = 0) const = 0;
  
  /** setString **/
  virtual bool setString (std::string& content, int index = 0) = 0;

}; // class Delegate


/**********************************************************************************************
 * SAYLGenericDelegate
 *********************************************************************************************/
template <class Type>
class SAYLGenericDelegate : public Delegate {
public :
  /** getValue **/
  virtual Type getValue (int index = 0) const = 0;
  
  /** setValue **/
  virtual bool setValue (Type, int index = 0) = 0;

  /** getTypeName **/
  const std::string getTypeName (void) const;

  /** isReadOnly **/
  virtual bool isReadOnly (void) const = 0;

  /** isIndexed **/
  virtual bool isIndexed (void) const = 0;

  /** getElementsCount **/
  virtual int getElementsCount (void) const = 0;

  /** getString **/
  const std::string getString (int index) const;
  
  /** setString **/
  bool setString (std::string& content, int index);
  
protected :
  /** getDefaultValue **/
  Type getDefaultValue (void) const;

}; // class SAYLGenericDelegate


/**********************************************************************************************
 * SAYLGenericDelegateMethod
 *********************************************************************************************/  
template <class Class, class Type>
class SAYLGenericDelegateMethod : public SAYLGenericDelegate <Type> {
public:
  /** typedefs **/
  typedef Type (Class::*getter) () const;
  typedef bool (Class::*setter) (Type);
  
  /** Constructor **/
  SAYLGenericDelegateMethod (Class *o, getter g, setter s = 0) :
    __object (o), __getter (g), __setter(s)
  { }
  
  /** Destructor **/
  ~SAYLGenericDelegateMethod () {}
  
  /** getValue **/
  Type getValue (int index = 0) const {
    if (__getter != 0) {
      return (__object->*__getter)();
    } else {
      return SAYLGenericDelegate<Type>::getDefaultValue ();
    }
  }
  
  /** setValue **/
  bool setValue (Type value, int index = 0) {
    if (__setter != 0) {      
      return (__object->*__setter)(value);
    } else {
      return false;
    }
  }
  
  /** isReadOnly **/
  bool isReadOnly (void) const {
    return (__setter == 0);
  }

  /** isIndexed **/
  bool isIndexed (void) const {
    return false;
  }

  /** getElementsCount **/
  int getElementsCount (void) const {
    return 1;
  }

private:
  Class *__object;
  getter __getter;
  setter __setter;
  
}; // class SAYLGenericDelegateMethod


/**********************************************************************************************
 * SAYLGenericDelegateMethodIndexed
 *********************************************************************************************/
template <class Class, class Type>
class SAYLGenericDelegateMethodIndexed : public SAYLGenericDelegate <Type> {
public:
  /** typedefs **/  
  typedef int  (Class::*count) (void) const;
  typedef Type (Class::*getter) (int) const;
  typedef bool (Class::*setter) (int, Type);
  
  /** Constructor **/
  SAYLGenericDelegateMethodIndexed (Class *o, count c, getter g, setter s = 0) :
    __object (o), __count(c), __getter (g), __setter(s)
  {}
  
  /** Destructor **/
  ~SAYLGenericDelegateMethodIndexed () {}
  
  /** getValue **/
  Type getValue (int index = 0) const {
    if (__getter != 0 && __count != 0 && index < (__object->*__count) ()) {
      return (__object->*__getter)(index);
    } else {
      return SAYLGenericDelegate<Type>::getDefaultValue ();
    }
  }
  
  /** setValue **/
  bool setValue (Type value, int index = 0) {
    if (__setter != 0 && __count != 0 && index < (__object->*__count) ()) {
      return (__object->*__setter)(index, value);
    } else {
      return false;
    }
  }
  
  /** isReadOnly **/
  bool isReadOnly (void) const {
    return (__setter == 0);
  }

  /** isIndexed **/
  bool isIndexed (void) const {
    return true;
  }

  /** getElementsCount **/
  int getElementsCount (void) const {
    if (__count != 0) {
      return (__object->*__count) ();
    } else {
      return 1;
    }
  }
  
private:
  Class *__object;  
  count  __count;
  getter __getter;
  setter __setter;
  
}; // class SAYLGenericDelegateMethodIndexed

}  // namespace SAYL

#endif
