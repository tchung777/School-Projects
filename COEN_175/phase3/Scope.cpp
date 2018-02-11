#include "Scope.h"
#include <cassert>
#include <iostream>

Scope::Scope(Scope *enclosing): _enclosing(enclosing){}

Scope* Scope::enclosing() const {
	return _enclosing;
}

const Symbols& Scope::symbols() const {
	return _symbols;
}

void Scope::insert(Symbol *sym) {
		assert(find(sym->getName()) == nullptr); //Better not already be inserted
		_symbols.push_back(sym); //Insert the given symbol into our symbol table.
}

void Scope::remove(const string &name) {
	for(unsigned i = 0; i < _symbols.size(); i++)
		if(_symbols[i]->getName() == name)
			_symbols.erase(_symbols.begin() + i);
}

Symbol* Scope::find(const string &name) const {
	for(unsigned i = 0; i < _symbols.size(); i++) {
		if(name == _symbols[i]->getName())
			return _symbols[i];
	}

	return nullptr;
}

Symbol* Scope::lookup(const string &name) const {
	Symbol *symbol;
	if ((symbol = find(name)) != nullptr)
		return symbol;

	return _enclosing != nullptr ? _enclosing->lookup(name) : nullptr;
}

void Scope::printAll() {
	std::cout << "Symbol element include:" << std::endl;
	for(int i = 0; i < _symbols.size(); i++) {
		std::cout << _symbols[i]->getName() << std::endl;
	}
}
