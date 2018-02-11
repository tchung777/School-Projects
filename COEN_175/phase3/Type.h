#ifndef TYPE_H
#define TYPE_H
#include <ostream>
#include <vector>

typedef std::vector<class Type> Parameters;
class Type {
	int _specifier;
	unsigned _indirection, _length;
	enum {ARRAY, SCALAR, FUNCTION, ERROR} _kind;
	Parameters* _parameters;

	public:
		Type();
		Type(int specifier, unsigned indirection = 0);
		Type(int specifier, unsigned indirection, unsigned length);
		Type(int specifier, unsigned indirection, Parameters *params);
		bool operator==(const Type &rhs) const;
		bool operator!=(const Type &rhs) const;
		int specifier() const;

		bool isArray() const;
		bool isScalar() const;
		bool isFunction() const;
		bool isError() const;

		unsigned length() const;
		unsigned indirection() const;
		Parameters *parameters() const;

};

std::ostream &operator << (std::ostream &ostr, const Type &type);

#endif /* SYMBOL_H */
