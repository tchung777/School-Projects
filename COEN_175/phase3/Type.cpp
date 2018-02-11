#include "Type.h"
#include "tokens.h"
#include <cassert>

using namespace std;

Type::Type()
: _kind(ERROR)
{
}
Type::Type(int specifier, unsigned indirection)
: _specifier(specifier), _indirection(indirection), _kind(SCALAR)
{
}

Type::Type(int specifier, unsigned indirection, unsigned length)
: _specifier(specifier), _indirection(indirection), _length(length)
{
	_kind = ARRAY;
}

Type::Type(int specifier, unsigned indirection, Parameters *params)
: _specifier(specifier), _indirection(indirection), _parameters(params)
{
	_kind = FUNCTION;
}

int Type::specifier() const {
	return _specifier;
}

bool Type::operator ==(const Type &rhs) const {
	if(_kind != rhs._kind)
	return false;
	if(_kind == ERROR)
	return true;
	if(_specifier != rhs._specifier)
	return false;
	if(_indirection != rhs._indirection)
	return false;
	if(_kind == SCALAR)
	return true;

	if(_kind == ARRAY)
	return _length == rhs._length;

	if(_parameters == rhs._parameters)
	return true;

	if (_parameters == nullptr || rhs._parameters == nullptr)
	return false;

	return *_parameters == *rhs._parameters;
}

bool Type::operator !=(const Type &rhs) const {
	return !operator ==(rhs);
}

bool Type::isArray() const {
	return _kind == ARRAY;
}

bool Type::isScalar() const {
	return _kind == SCALAR;
}

bool Type::isFunction() const {
	return _kind == FUNCTION;
}

bool Type::isError() const {
	return _kind == ERROR;
}

unsigned Type::length() const {
	assert(_kind == ARRAY);
	return _length;
}

unsigned Type::indirection() const {
	return _indirection;
}

Parameters *Type::parameters() const {
	return _parameters;
}

std::ostream &operator<<(std::ostream &ostr, const Type &t) {
	unsigned i;
	if(t.isError())
	ostr << "error";
	else {
		if(t.specifier() == INT)
		ostr << "int";
		else if(t.specifier() == CHAR)
		ostr << "char";
		else if(t.specifier() == VOID)
		ostr << "void";
		else ostr << "illegal specifier";

		if(t.indirection() > 0) {
			ostr << " ";
			for(i=0; i < t.indirection(); i++)
			ostr << "*";
		}

		if(t.isArray())
		ostr << "[" << t.length() << "]";
		else if(t.isFunction()) {
			ostr << "(";
			if(t.parameters() -> size() == 0)
				ostr << "void";
			else {
				for(unsigned i = 0; i < t.parameters()->size();i++) {
					if(i > 0)
					ostr << ", ";
					ostr << (*t.parameters())[i];
				}
			}
			ostr << ")";
		}
	}
	return ostr;
}
