#include "Symbol.h"

using std::string;

Symbol::Symbol(const string &name, const Type &type)
	: _name(name), _type(type), _attributes(0)
	{
	}

/*
 * Function:	Symbol::name (accessor)
 *
 * Description:	Return the name of this symbol.
 */

const string &Symbol::getName() const
{
    return _name;
}


/*
 * Function:	Symbol::type (accessor)
 *
 * Description:	Return the type of this symbol.
 */

const Type &Symbol::getType() const
{
    return _type;
}
