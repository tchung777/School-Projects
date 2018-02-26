/*
* File:	Type.cpp
*
* Description:	This file contains the member function definitions for
*		types in Simple C.  A type is either a scalar type, an
*		array type, or a function type.
*
*		Note that we simply don't like putting function definitions
*		in the header file.  The header file is for the interface.
*		Actually, we prefer opaque pointer types in C where you
*		don't even get to see what's inside, much less touch it.
*		But, C++ lets us have value types with access control
*		instead of just always using pointer types.
*
*		Extra functionality:
*		- equality and inequality operators
*		- predicate functions such as isArray()
*		- stream operator
*		- the error type
*/

# include <cassert>
# include "Type.h"
# include "tokens.h"
# include "nullptr.h"

using namespace std;


/*
* Function:	Type::Type (constructor)
*
* Description:	Initialize this type as an error type.
*/

Type::Type()
: _kind(ERROR)
{
}


/*
* Function:	Type::Type (constructor)
*
* Description:	Initialize this type object as a scalar type.
*/

Type::Type(int specifier, unsigned indirection)
: _specifier(specifier), _indirection(indirection), _kind(SCALAR)
{
}


/*
* Function:	Type::Type (constructor)
*
* Description:	Initialize this type object as an array type.
*/

Type::Type(int specifier, unsigned indirection, unsigned length)
: _specifier(specifier), _indirection(indirection), _length(length)
{
  _kind = ARRAY;
}


/*
* Function:	Type::Type (constructor)
*
* Description:	Initialize this type object as a function type.
*/

Type::Type(int specifier, unsigned indirection, Parameters *parameters)
: _specifier(specifier), _indirection(indirection), _parameters(parameters)
{
  assert(_parameters != nullptr);
  _kind = FUNCTION;
}


/*
* Function:	Type::operator ==
*
* Description:	Return whether another type is equal to this type.  The
*		parameter lists are checked for function types, which C++
*		makes so easy.  (At least, it makes something easy!)
*/

bool Type::operator ==(const Type &rhs) const
{
  if (_kind != rhs._kind)
  return false;

  if (_kind == ERROR)
  return true;

  if (_specifier != rhs._specifier)
  return false;

  if (_indirection != rhs._indirection)
  return false;

  if (_kind == SCALAR)
  return true;

  if (_kind == ARRAY)
  return _length == rhs._length;

  if (_parameters->_varargs != rhs._parameters->_varargs)
  return false;

  return _parameters->_types == rhs._parameters->_types;
}


/*
* Function:	Type::operator !=
*
* Description:	Well, what do you think it does?  Why can't the language
*		generate this function for us?  Because they think we want
*		it to do something else?  Yeah, like that'd be a good idea.
*/

bool Type::operator !=(const Type &rhs) const
{
  return !operator ==(rhs);
}


/*
* Function:	Type::isArray
*
* Description:	Return whether this type is an array type.
*/

bool Type::isArray() const
{
  return _kind == ARRAY;
}


/*
* Function:	Type::isScalar
*
* Description:	Return whether this type is a scalar type.
*/

bool Type::isScalar() const
{
  return _kind == SCALAR;
}


/*
* Function:	Type::isFunction
*
* Description:	Return whether this type is a function type.
*/

bool Type::isFunction() const
{
  return _kind == FUNCTION;
}


/*
* Function:	Type::isError
*
* Description:	Return whether this type is an error type.
*/

bool Type::isError() const
{
  return _kind == ERROR;
}


/*
* Function:	Type::specifier (accessor)
*
* Description:	Return the specifier of this type.
*/

int Type::specifier() const
{
  return _specifier;
}


/*
* Function:	Type::indirection (accessor)
*
* Description:	Return the number of levels of indirection of this type.
*/

unsigned Type::indirection() const
{
  return _indirection;
}


/*
* Function:	Type::length (accessor)
*
* Description:	Return the length of this type, which must be an array
*		type.  Is there an better way than calling assert?  There
*		certainly isn't an easier way.
*/

unsigned Type::length() const
{
  assert(_kind == ARRAY);
  return _length;
}


/*
* Function:	Type::parameters (accessor)
*
* Description:	Return the parameters of this type, which must be a
*		function type.
*/

Parameters *Type::parameters() const
{
  assert(_kind == FUNCTION);
  return _parameters;
}

Type Type::promote() const {
  if(*this == Type(CHAR, 0))
    return Type(INT, 0);
  if(this->isArray())
    return Type(_specifier, _indirection + 1);
  return *this;
}

bool Type::isValue() const {
  Type t = promote();
  return (t.isInteger() || t.isPointer());
}

bool Type::isInteger() const {
  return *this == Type(INT, 0);
}

bool Type::isPointer() const {
  return (_kind == SCALAR && _indirection > 0) || _kind == ARRAY; // maybe take out || _kind == ARRAY
}

bool Type::isCompatibleWith(const Type &that) const {
  Type promote_this = this->promote();
  Type promote_that = that.promote();

  if (promote_this.isPointer() && (promote_that == Type(VOID,1)))
    return true;

  else if (promote_that.isPointer() && (promote_this == Type(VOID,1)))
    return true;

  return (isValue() && that.isValue()) && (promote_this == promote_that);
}

/*Type Type::deref() const
{
  assert(_kind == SCALAR && _indirection > 0);
  return Type(_specifier, _indirection - 1);
}*/

/*
* Function:	operator <<
*
* Description:	Write a type to the specified output stream.  At least C++
*		let's us do some cool things.
*/

ostream &operator <<(ostream &ostr, const Type &type)
{
  unsigned i;


  if (type.isError())
  ostr << "error";

  else {
    if (type.specifier() == INT)
    ostr << "int";
    else if (type.specifier() == CHAR)
    ostr << "char";
    else if (type.specifier() == VOID)
    ostr << "void";
    else
    ostr << type.specifier();		// shouldn't happen

    if (type.indirection() > 0) {
      ostr << " ";

      for (i = 0; i < type.indirection(); i ++)
      ostr << "*";
    }

    if (type.isArray())
    ostr << "[" << type.length() << "]";

    else if (type.isFunction()) {
      ostr << "(";

      if (type.parameters() != nullptr) {
        if (type.parameters()->_types.size() == 0)
        ostr << "void";
        else
        for (i = 0; i < type.parameters()->_types.size(); i ++) {
          if (i > 0)
          ostr << ", ";
          ostr << type.parameters()->_types[i];
        }

        if (type.parameters()->_varargs)
        ostr << ", ...";
      }

      ostr << ")";
    }
  }

  return ostr;
}
