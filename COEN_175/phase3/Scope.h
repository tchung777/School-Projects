#ifndef SCOPE_H
#define SCOPE_H

#include "Symbol.h"
#include <vector>

typedef std::vector<Symbol *> Symbols;

class Scope {
	typedef std::string string;
	Scope *_enclosing;
	Symbols _symbols;

	public:
		Scope(Scope *enclosing = 0);
		void insert(Symbol *sym);
		void remove(const string &name);
		void printAll();
		Symbol *find(const string &name) const;
		Symbol *lookup(const string &name) const;
		Scope *enclosing() const;
		const Symbols &symbols() const;
};

#endif /* SCOPE_H */
