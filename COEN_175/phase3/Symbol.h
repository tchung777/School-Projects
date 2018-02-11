#ifndef SYMBOL_H
#define SYMBOL_H
# include <string>
# include "Type.h"

class Symbol {
	typedef std::string string;
	string _name;
	Type _type;

	public:
		int _attributes;

		Symbol(const string &name, const Type &type);

		const string &getName() const;
		const Type &getType() const;

};

#endif /* SYMBOL_H */
