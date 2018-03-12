/*
* File:	generator.cpp
*
* Description:	This file contains the public and member function
*		definitions for the code generator for Simple C.
*
*		Extra functionality:
*		- putting all the global declarations at the end
*/

# include <sstream>
# include <iostream>
# include "Label.h"
# include "generator.h"
# include "machine.h"

using namespace std;

static unsigned maxargs;
static int temp_offset;
Label *retLbl;

void assignTemp(Expression * expr) {
  stringstream ss;
  temp_offset -= expr->type().size();
  ss << temp_offset << "(%EBP)";
  expr->_operand = ss.str();
}

/*
* Function:	operator <<
*
* Description:	Convenience function for writing the operand of an
*		expression.
*/

ostream &operator <<(ostream &ostr, Expression *expr)
{
  return ostr << expr->_operand;
}


/*
* Function:	Identifier::generate
*
* Description:	Generate code for an identifier.  Since there is really no
*		code to generate, we simply update our operand.
*/

void Identifier::generate()
{
  stringstream ss;


  if (_symbol->_offset != 0)
  ss << _symbol->_offset << "(%ebp)";
  else
  ss << global_prefix << _symbol->name();

  _operand = ss.str();
}


/*
* Function:	Number::generate
*
* Description:	Generate code for a number.  Since there is really no code
*		to generate, we simply update our operand.
*/

void Number::generate()
{
  stringstream ss;


  ss << "$" << _value;
  _operand = ss.str();
}


# if STACK_ALIGNMENT == 4

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

# else

/*
* If the stack has to be aligned to a certain size before a function call
* then we cannot push the arguments in the order we see them.  If we had
* nested function calls, we cannot guarantee that the stack would be
* aligned.
*
* Instead, we must know the maximum number of arguments so we can compute
* the size of the frame.  Again, we cannot just move the arguments onto
* the stack as we see them because of nested function calls.  Rather, we
* have to generate code for all arguments first and then move the results
* onto the stack.  This will likely cause a lot of spills.
*
* For now, since each argument is going to be either a number of in
* memory, we just load it into %eax and then move %eax onto the stack.
*/

void Call::generate()
{
  if (_args.size() > maxargs)
  maxargs = _args.size();

  for (int i = _args.size() - 1; i >= 0; i --) {
    _args[i]->generate();
    cout << "\tmovl\t" << _args[i] << ", %eax" << endl;
    cout << "\tmovl\t%eax, " << i * SIZEOF_ARG << "(%esp)" << endl;
  }

  cout << "\tcall\t" << global_prefix << _id->name() << endl;
}

# endif


/*
* Function:	Assignment::generate
*
* Description:	Generate code for this assignment statement, in which the
*		right-hand side is an integer literal and the left-hand
*		side is an integer scalar variable.  Actually, the way
*		we've written things, the right-side can be a variable too.
*/

void Assignment::generate()
{
  bool indirect;
  _left->generate(indirect);
  _right->generate();

  if(indirect) {
    cout << "\tmovl\t" << _right << ", %eax" << endl;
    cout << "\tmovl\t" << _left << ", %ecx" << endl;

    if(_left->type().size() == 1)
    cout << "\tmovb\t%al, (%ecx)" << endl;
    else
    cout << "\tmovl\t%eax, (%ecx)" << endl;
  } else {
    cout << "\tmovl\t" << _right << ", %eax" << endl;
    if(_left -> type().size() == 1)
    cout << "\tmovb\t%al," << _left << endl;
  }
}


/*
* Function:	Block::generate
*
* Description:	Generate code for this block, which simply means we
*		generate code for each statement within the block.
*/

void Block::generate()
{
  for (unsigned i = 0; i < _stmts.size(); i ++)
  _stmts[i]->generate();
}


/*
* Function:	Function::generate
*
* Description:	Generate code for this function, which entails allocating
*		space for local variables, then emitting our prologue, the
*		body of the function, and the epilogue.
*/

void Function::generate()
{
  int offset = 0;


  /* Generate our prologue. */

  allocate(offset);
  cout << global_prefix << _id->name() << ":" << endl;
  cout << "\tpushl\t%ebp" << endl;
  cout << "\tmovl\t%esp, %ebp" << endl;
  cout << "\tsubl\t$" << _id->name() << ".size, %esp" << endl;


  /* Generate the body of this function. */


  maxargs = 0;
  temp_offset = offset;
  _body->generate();
  offset = temp_offset;

  offset -= maxargs * SIZEOF_ARG;

  while ((offset - PARAM_OFFSET) % STACK_ALIGNMENT)
  offset --;


  /* Generate our epilogue. */

  cout << "\tmovl\t%ebp, %esp" << endl;
  cout << "\tpopl\t%ebp" << endl;
  cout << "\tret" << endl << endl;

  cout << "\t.globl\t" << global_prefix << _id->name() << endl;
  cout << "\t.set\t" << _id->name() << ".size, " << -offset << endl;

  cout << endl;
}


/*
* Function:	generateGlobals
*
* Description:	Generate code for any global variable declarations.
*/

void generateGlobals(const Symbols &globals)
{
  if (globals.size() > 0)
  cout << "\t.data" << endl;

  for (unsigned i = 0; i < globals.size(); i ++) {
    cout << "\t.comm\t" << global_prefix << globals[i]->name();
    cout << ", " << globals[i]->type().size();
    cout << ", " << globals[i]->type().alignment() << endl;
  }
}

static vector<string> labelBuff;

 // *EXPRESSIONS*
void LogicalAnd::generate() {
  _left->generate();
  assignTemp(this);//_operand = getTemp();

  string lval = _left->_operand;
  string rval;
  Label andTrueLabel = Label();
  Label andFalseLabel = Label();

  if(lval[0] == '$') {
      cout << "\tmovl\t" << lval << ", %eax" << endl;
      lval = "%eax";
  }

  cout << "\tcmpl\t$0, " << lval << endl;
  cout << "\tje\t" << andFalseLabel << endl;

  _right->generate();
  rval = _right->_operand;

  if (rval[0] == '$') {
      cout << "\tmovl\t" << rval << ", %eax" << endl;
      rval = "%eax";
  }

  cout << "\tcmpl\t$0, " << rval << endl;
  cout << "\tje\t" << andFalseLabel << endl;
  cout << "\tmovl\t$1, %eax" << endl;
  cout << "\tjmp\t" << andTrueLabel << endl;

  cout << andFalseLabel << ":" << endl;
  cout << "\tmovl\t$0, %eax" << endl;

  cout << andTrueLabel << ":" << endl;
  cout << "\tmovl\t%eax, " << _operand << endl;

}

void LogicalOr::generate() {
  _left->generate();
  assignTemp(this);

  string lval = _left->_operand;
  string rval;
  Label orTrueLabel = Label();
  Label orFalseLabel = Label();

  if(lval[0] == '$') {
      cout << "\tmovl\t" << lval << ", %eax" << endl;
      lval = "%eax";
  }

  cout << "\tcmpl\t$0, " << lval << endl;
  cout << "\tjne\t" << orTrueLabel << endl;

  _right->generate();
  rval = _right->_operand;

  if (rval[0] == '$') {
      cout << "\tmovl\t" << rval << ", %eax" << endl;
      rval = "%eax";
  }

  cout << "\tcmpl\t$0, " << rval << endl;
  cout << "\tjne\t" << orTrueLabel << endl;

  cout << "\tjmp\t" << orFalseLabel << endl;

  cout << orTrueLabel << ":" << endl;
  cout << "\tmovl\t$1, %eax" << endl;

  cout << orFalseLabel << ":" << endl;
  cout << "\tmovl\t$0, %eax" << endl;


}

void LessThan::generate() {
  _left->generate();
  _right->generate();
  assignTemp(this);

  cout << "\tmovl\t" << _left << ", %eax" <<endl;
  cout << "\tcmpl\t" << _right << ", %eax" << endl;
  cout << "\tsetl\t%al" << endl; //opcode
  cout << "\tmovzbl\t%al, %eax" << endl;
  cout << "\tmovl\t%eax, " << _operand << endl;
}

void LessOrEqual::generate() {
  _left->generate();
  _right->generate();
  assignTemp(this);

  cout << "\tmovl\t" << _left << ", %eax" <<endl;
  cout << "\tcmpl\t" << _right << ", %eax" << endl;
  cout << "\tsetle\t%al" << endl; //opcode
  cout << "\tmovzbl\t%al, %eax" << endl;
  cout << "\tmovl\t%eax, " << _operand << endl;
}

void GreaterThan::generate() {
  _left->generate();
  _right->generate();
  assignTemp(this);

  cout << "\tmovl\t" << _left << ", %eax" <<endl;
  cout << "\tcmpl\t" << _right << ", %eax" << endl;
  cout << "\tsetg\t%al" << endl; //opcode
  cout << "\tmovzbl\t%al, %eax" << endl;
  cout << "\tmovl\t%eax, " << _operand << endl;
}

void GreaterOrEqual::generate() {
  _left->generate();
  _right->generate();
  assignTemp(this);

  cout << "\tmovl\t" << _left << ", %eax" <<endl;
  cout << "\tcmpl\t" << _right << ", %eax" << endl;
  cout << "\tsetge\t%al" << endl; //opcode
  cout << "\tmovzbl\t%al, %eax" << endl;
  cout << "\tmovl\t%eax, " << _operand << endl;
}

void Equal::generate() {
  _left->generate();
  _right->generate();
  assignTemp(this);

  cout << "\tmovl\t" << _left << ", %eax" <<endl;
  cout << "\tcmpl\t" << _right << ", %eax" << endl;
  cout << "\tsete\t%al" << endl; //opcode
  cout << "\tmovzbl\t%al, %eax" << endl;
  cout << "\tmovl\t%eax, " << _operand << endl;
}

void NotEqual::generate() {
  _left->generate();
  _right->generate();
  assignTemp(this);

  cout << "\tmovl\t" << _left << ", %eax" <<endl;
  cout << "\tcmpl\t" << _right << ", %eax" << endl;
  cout << "\tsetne\t%al" << endl; //opcode
  cout << "\tmovzbl\t%al, %eax" << endl;
  cout << "\tmovl\t%eax, " << _operand << endl;
}

void Add::generate() {
  _left->generate();
  _right->generate();
  assignTemp(this);

  cout << "\tmovl\t" << _left << ", %eax" << endl;
  cout << "\taddl\t" << _right << ", %eax" << endl;
  cout << "\tmovl\t%eax, " << _operand << endl;
}

void Subtract::generate() {
  _left->generate();
  _right->generate();
  assignTemp(this);

  cout << "\tmovl\t" << _left << ", %eax" << endl;
  cout << "\tsubl\t" << _right << ", %eax" << endl;
  cout << "\tmovl\t%eax, " << _operand << endl;
}

void Multiply::generate() {
  _left->generate();
  _right->generate();
  assignTemp(this);

  cout << "\tmovl\t" << _left << ", %eax" << endl;
  cout << "\timull\t" << _right << ", %eax" << endl;
  cout << "\tmovl\t%eax, " << _operand << endl;
}

void Divide::generate() {
  _left->generate();
  _right->generate();
  assignTemp(this);

  cout << "\tmovl\t" << _left << ", %eax" << endl;
  cout << "\tmovl\t" << _right << ", %ecx" << endl;
  cout << "\tcltd" << endl;

  cout << "\tidivl\t%ecx" << endl;
  cout << "\tmovl\t%eax, " << _operand << endl;
}

void Remainder::generate() {
  _left->generate();
  _right->generate();
  assignTemp(this);

  cout << "\tmovl\t" << _left << ", %eax" << endl;
  cout << "\tmovl\t" << _right << ", %ecx" << endl;
  cout << "\tcltd" << endl;

  cout << "\tidivl\t%ecx" << endl;
  cout << "\tmovl\t%edx, " << _operand << endl;
}

// *EXPRESSIONS ENDS HERE-------------


void Not::generate() {
  _expr->generate();
  assignTemp(this);

  cout << "\tmovl\t" << _expr << ", %eax" << endl;
  cout << "\tcmpl\t$0, %eax" << endl;

  cout << "\tsete\t%al" << endl;
  cout << "\tmovzbl\t%al, %eax" << endl;
  cout << "\tmovl\t%eax, " << _operand << endl;
}

void Negate::generate() {
  _expr->generate();

	cout << "\tmovl\t" << _expr << ", %eax" << endl;
	cout << "\tnegl\t%eax" << endl;

	assignTemp(this);

	cout << "\tmovl\t%eax, " << _operand << endl;
}

void Dereference::generate(){
    _expr->generate();
    assignTemp(this);

    int exprLength = _expr->type().size();

    if (exprLength == 1) {
        cout << "\tmovb\t" << _expr << ", %al" << endl;
        cout << "\tmovb\t%al, " << _operand  << endl;
    } else if (exprLength == 4) {
        cout << "\tmovl\t" << _expr << ", %eax" << endl;
        cout << "\tmovl\t%eax, " << _operand  << endl;
    }
}

void Dereference::generate(bool &indirect) {
    _expr->generate();
    assignTemp(this);
    indirect = true;

    int exprLength = _expr->type().size();

    if (exprLength == 1) {
        cout << "\tmovb\t" << _expr << ", %al" << endl;
        cout << "\tmovb\t%al, " << _operand  << endl;
    } else if (exprLength == 4) {
        cout << "\tmovl\t" << _expr << ", %eax" << endl;
        cout << "\tmovl\t%eax, " << _operand  << endl;
    }
}

void Address::generate() {
    bool indirect;
    _expr->generate(indirect);

    if (_expr->_operand.size() >= 5 && _expr->_operand.substr(0, 4) == "$.LC") {
        _operand = _expr->_operand;
    } else {
        assignTemp(this);

        if (indirect) {
            cout << "\tleal\t(%eax), %eax" << endl;
            cout << "\tmovl\t%eax, " << _operand << endl;
        } else {
            cout << "\tleal\t" << _expr << ", %eax" << endl;
            cout << "\tmovl\t%eax, " << _operand << endl;
        }
    }
}

void Promote::generate() {
    _expr->generate();
    if (_expr->type().size() == 1) {
        cout << "\tmovl\t%eax, " << _expr << ", %eax" << endl;
        cout << "\tmovsbl\t%al, %eax" << endl;
        cout << "\tmovl\t%eax, " << _operand << endl;
    }
}

void Return::generate() {
    _expr->generate();
    cout << "\tmovl\t" << _expr << ", %eax" << endl;
    cout << "\tjmp\t" << retLbl<< endl;
}

void If::generate() {
    _expr->generate();
    string exprVal = _expr->_operand;
    Label endLabel = Label();
    Label elseLabel;

    if(exprVal[0] == '$') {
        cout << "\tmovl\t" << exprVal << ", %eax" << endl;
        exprVal = "%eax";
    }

    cout << "\tcmpl\t$0, " << exprVal << endl;

    if (_elseStmt != nullptr) {
        elseLabel = Label();

        cout << "\tje\t" << elseLabel << endl;
        _thenStmt->generate();
        cout << "\tjmp\t" << endLabel << endl;

        cout << elseLabel << ":" << endl;
        _elseStmt->generate();
    } else {
        cout << "\tje\t" << endLabel << endl;
        _thenStmt->generate();
    }

    cout << endLabel << ":" << endl;
}

void For::generate() {
    Label loopLabel = Label();
    Label condLabel = Label();

    _init->generate();

    cout << "\tjmp\t" << condLabel << endl;
    cout << loopLabel << ":" << endl;
    _stmt->generate();
    _incr->generate();
    cout << condLabel << ":" << endl;
    _expr->generate();

    string exprVal = _expr->_operand;

    if (exprVal[0] == '$') {
        cout << "\tmovl\t" << exprVal << ", %eax" << endl;
        exprVal = "%eax";
    }

    cout << "\tcmpl\t$0, " << exprVal << endl;
    cout << "\tjne\t" << loopLabel << endl;
}

void While::generate() {
    Label loopLabel = Label();
    Label condLabel = Label();

    cout << "\tjmp\t" << condLabel << endl;
    cout << loopLabel << ":" << endl;
    _stmt->generate();
    cout << condLabel << ":" << endl;
    _expr->generate();

    string exprVal = _expr->_operand;

    if (exprVal[0] == '$') {
        cout << "\tmovl\t" << exprVal << ", %eax" << endl;
        exprVal = "%eax";
    }

    cout << "\tcmpl\t$0, " << exprVal << endl;
    cout << "\tjne\t" << loopLabel << endl;
}

void String::generate() {
    Label stringLabel = Label();
    stringstream ss;
    ss << stringLabel << ":" << "\t.asciz\t" << _value << endl;
    labelBuff.push_back(ss.str());

    //_operand = stringLabel;
}
