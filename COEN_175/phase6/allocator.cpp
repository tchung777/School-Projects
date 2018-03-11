#include "Tree.h"
#include "tokens.h"
#include <iostream>
#include <cassert>

using namespace std;

unsigned Type::size() const
{
  unsigned count;


  assert(_kind != FUNCTION && _kind != ERROR);
  count = (_kind == ARRAY ? _length : 1);

  if (_indirection > 0)
  return count * SIZEOF_PTR;

  if (_specifier == INT)
  return count * SIZEOF_INT;

  if (_specifier == CHAR)
  return count * SIZEOF_CHAR;

  return 0;
}

unsigned Type::alignment() const
{
  assert(_kind != FUNCTION && _kind != ERROR);

  if (_indirection > 0)
  return ALIGNOF_PTR;

  if (_specifier == CHAR)
  return ALIGNOF_CHAR;

  if (_specifier == INT)
  return ALIGNOF_INT;

  return 0;
}

void Block::allocate(int &offset) const {
  int temp, saved;
  unsigned i;
  Symbols symbols;


  symbols = _decls->symbols();

  for (i = 0; i < symbols.size(); i ++)
    if (symbols[i]->_offset == 0) {
      offset -= symbols[i]->type().size();
      symbols[i]->_offset = offset;
    }

  saved = offset;

  for (i = 0; i < _stmts.size(); i ++) {
    temp = saved;
    _stmts[i]->allocate(temp);
    offset = min(offset, temp);
  }
}

void Function::allocate(int &offset) const
{
  Parameters *params;
  Symbols symbols;

  params = _id->type().parameters();
  symbols = _body->declarations()->symbols();
  offset = PARAM_OFFSET;

  for (unsigned i = 0; i < params->_types.size(); i ++) {
    symbols[i]->_offset = offset;
    offset += SIZEOF_ARG;
  }

  offset = 0;
  _body->allocate(offset);
}

void While::allocate(int &offset) const
{
    _stmt->allocate(offset);
}

void For::allocate(int &offset) const
{
    _stmt->allocate(offset);
}

void If::allocate(int &offset) const
{
  int saved, temp;


  saved = offset;
  _thenStmt->allocate(offset);

  if (_elseStmt != nullptr) {
    temp = saved;
    _elseStmt->allocate(temp);
    offset = min(offset, temp);
  }
}
