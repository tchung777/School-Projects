#include "Tree.h"
#include <iostream>
#include <sstream>

using namespace std;
static unsigned maxargs;

ostream &operator <<(ostream &ostr, Expression *expr)
{
    return ostr << expr->_operand;
}

void Function::generate()
{
  int offset = 0;
  cout << "\t.globl\t" << global_prefix << _id->name() << endl;
  // do function prologue
  allocate(offset);
  cout << global_prefix << _id->name() << ":" << endl;
  cout << "\tpushl\t%ebp" << endl;
  cout << "\tmovl\t%esp, %ebp" << endl;
  cout << "\tsubl\t$" << -offset << ", %esp" << endl;
  _body->generate();

  // do function epilogue
  cout << "\tmovl\t%ebp, %esp" << endl;
  cout << "\tpopl\t%ebp" << endl;
  cout << "\tret" << endl;
}

void Block::generate()
{
  for(unsigned i = 0; i < _stmts.size() ; i++) {
    _stmts[i]->generate();
  }
}

void Call::generate()
{
  unsigned numBytes = 0;


  for (int i = _args.size() - 1; i >= 0; i --) {
    _args[i]->generate();
    cout << "\tpushl\t" << _args[i] << endl;
    numBytes += _args[i]->type().size();
  }

  cout << "\tcall\t" << global_prefix << _id->name() << endl;

  if (numBytes > 0)
  cout << "\taddl\t$" << numBytes << ", %esp" << endl;
}

void Identifier::generate() {
  stringstream ss;

  if(_symbol->_offset != 0) {
    ss << _symbol->_offset << "(%ebp)";
  }
  else {
	   ss << global_prefix << _symbol->name();
  }
  _operand = ss.str();
}

void Number::generate() {
  stringstream ss;

  ss << "$" << _value;
  _operand = ss.str();
}

void Assignment::generate() {
  _left->generate();
  _right->generate();

  cout << "\tmovl\t" << _right << ", %eax" << endl;
  cout << "\tmovl \t%eax, " << _left << endl;
}

void generateGlobals(const Symbols &globals)
{
  if (globals.size() > 0)
    cout << "\t.data " << endl;

  for (unsigned i = 0; i < globals.size(); i ++) {
    cout << "\t.comm\t" << global_prefix << globals[i]->name();
    cout << ", " << globals[i]->type().size();
    cout << ", " << globals[i]->type().alignment() << endl;
  }
}
