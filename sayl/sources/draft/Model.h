#ifndef __SAYL_MODEL_H__
#define __SAYL_MODEL_H__

#include <string>
#include <map>

#include "Delegate.h"

namespace SAYL {

/**********************************************************************************************
 * Model
 *********************************************************************************************/
class Model {
public:
  /********************************************************************************************
   *  Iterator
   *******************************************************************************************/
  typedef std::map<std::string, Delegate *> Map;
  typedef std::map<std::string, Delegate *>::const_iterator MapIterator;
  
  class Iterator {
  protected :
    friend class Model; // Container

    /** Constructor **/
    Iterator (MapIterator start, MapIterator end) : inner (start), limit (end) {}
    
  public :
    /** Destructor **/
    ~Iterator () {}
    
    /** Next **/
    bool Next ();
    
    /** End **/
    bool End () const;
    
    /** Name **/
    std::string getName () const;
    
    /** TypeName **/
    std::string getTypeName () const;

    /** isReadOnly **/
    bool isReadOnly () const;

    /** isIndexed **/
    bool isIndexed () const;

    /** getElementsCount **/
    int getElementsCount () const;

    /** Model **/
    Model * getModel (int index = 0) const;
    
    /** getElements **/
    Iterator getElements (int index = 0) const; 

    /** Boolean **/
    bool getBoolean (int index = 0) const;
    bool setBoolean (bool value, int index = 0);

    /** Integer **/
    int getInteger (int index = 0) const;
    bool setInteger (int value, int index = 0);

    /** Long **/
    long getLong (int index = 0) const;
    bool setLong (long value, int index = 0);

    /** Float **/
    float getFloat (int index = 0) const;
    bool setFloat (float value, int index = 0);

    /** Double **/
    double getDouble (int index = 0) const;
    bool setDouble (double value, int index = 0);

    /** String **/
    std::string getString (int index = 0) const;
    bool setString (std::string value, int index = 0);
  
  private :
    const std::map <std::string, Delegate*> empty;    
    MapIterator inner;
    MapIterator limit;
    
  }; // class Iterator

  /** Constructor **/
  Model () {}
  
  /** Destructor **/
  virtual ~Model ();

  /** getElements **/
  Iterator getElements ();

  /** hasElement **/
  bool hasElement (const std::string name) const;

  /** getTypeName **/
  std::string getTypeName (const std::string name) const;

  /** isReadOnly **/
  bool isReadOnly (const std::string name) const;

  /** isIndexed **/
  bool isIndexed (const std::string name) const;

  /** getIndexCount **/
  int getIndexCount (const std::string name) const;

  /** getModel **/
  Model * getModel (const std::string name, int index = 0) const;

  /** Boolean **/
  bool getBoolean (const std::string name, int index = 0) const;
  bool setBoolean (const std::string name, bool value, int index = 0);

  /** Integer **/
  int getInteger (const std::string name, int index = 0) const;
  bool setInteger (const std::string name, int value, int index = 0);

  /** Long **/
  long getLong (const std::string name, int index = 0) const;
  bool setLong (const std::string name, long value, int index = 0);

  /** Float **/
  float getFloat (const std::string name, int index = 0) const;
  bool setFloat (const std::string name, float value, int index = 0);

  /** Double **/
  double getDouble (const std::string name, int index = 0) const;
  bool setDouble (const std::string name, double value, int index = 0);

  /** String **/
  std::string getString (const std::string name, int index = 0) const;
  bool setString (const std::string name, std::string value, int index = 0);
 
protected:
  /** Bind **/  
  template <class C, class T> inline void
  Bind (const std::string name, T (C::*getter)() const, bool (C::*setter)(T) = 0) {
    BindDelegate (name,(Delegate *) new SAYLGenericDelegateMethod <C,T> (dynamic_cast<C*> (this), getter, setter));
  }

  template <class C, class T> inline void
  Bind (const std::string name, int (C::*count) () const, T (C::*getter) (int) const, bool (C::*setter)(int, T) = 0) {
    BindDelegate (name, (Delegate *) new SAYLGenericDelegateMethodIndexed <C,T> (dynamic_cast<C*> (this), count, getter, setter));
  }
  
  /** Unbind **/
  void Unbind (const std::string &);

  /** BindDelegate **/
  virtual void BindDelegate (const std::string, Delegate *);
  
  /** UnbindDelegate **/
  virtual void UnbindDelegate (const std::string);
  
  /** getDelegate **/
  virtual Delegate * getDelegate (const std::string) const;

private :  
  /** getDelegateModel **/
  Delegate * getDelegateModel (char *) const;

  /** getDelegateAttribute **/
  Delegate * getDelegateAttribute (char *) const;

  std::map <std::string, Delegate*> elements;
    
}; // class Model

}  // namespace SAYL

#endif

