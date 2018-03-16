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
static vector<string> labelBuff;
static vector<Label> loop_labels;

void assignTemp(Expression * expr) {
  stringstream ss;
  temp_offset -= expr->type().size();
  ss << temp_offset << "(%ebp)";
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
  assignTemp(this);

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
  _right->generate();
  _left->generate(indirect);
  cout << "\tmovl\t" << _right << ", %eax" << endl;

  if(_left->type().size() != SIZEOF_INT){
    if(indirect){
      cout << "\tmovl\t" << _left << ", %ecx" << endl;
      cout << "\tmovb\t%al, (%ecx)" << endl;
    }else {
      cout << "\tmovb\t%al, " << _left << endl;
    }
  }else{
    if(indirect){
      cout << "\tmovl\t" << _left << ", %ecx" << endl;
      cout << "\tmovl\t%eax, (%ecx)" << endl;
    }else {
      cout << "\tmovl\t%eax, " << _left << endl;
    }

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
  retLbl = new Label();
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

  cout << *retLbl << ":" << endl;

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

 // *EXPRESSIONS*
 void Expression::generate(bool &indirect){
    indirect = false;
    generate();
}

void Expression::generate(){
    cerr << "Error in xpression::generate()" << endl;
}

void LogicalAnd::generate() {
  _left->generate();
  Label andLabel = Label();

  cout << "movl\t" << _left << ",%eax" << endl;
	cout << "cmpl\t$0,%eax" << endl;
	cout << "je\t" << andLabel << endl;

	_right->generate();
	assignTemp(this);
	cout << "movl\t" << _right << ",%eax" << endl;
	cout << "cmpl\t$0,%eax" << endl;
	cout << andLabel << ":" << endl;
	cout << "\tsetne\t%al" << endl;
	cout << "\tmovzbl\t%al,%eax" << endl;
	cout << "\tmovl\t%eax," << _operand << endl;

}

void LogicalOr::generate() {
  _left->generate();
  Label orLabel = Label();

  cout << "\tmovl\t" << _left << ",%eax" << endl;
	cout << "\tcmpl\t$0,%eax" << endl;
	cout << "\tjne\t" << orLabel << endl;

	_right->generate();
  assignTemp(this);
	cout << "\tmovl\t" << _right << ",%eax" << endl;
	cout << "\tcmpl\t$0,%eax" << endl;
	cout << orLabel << ":" << endl;
	cout << "\tsetne\t%al" << endl;
	cout << "\tmovzbl\t%al,%eax" << endl;
	cout << "\tmovl\t%eax," << _operand << endl;


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
  assignTemp(this);

	cout << "\tmovl\t" << _expr << ", %eax" << endl;
	cout << "\tnegl\t%eax" << endl;
	cout << "\tmovl\t%eax, " << _operand << endl;
}

void Dereference::generate(){
  _expr->generate();
  assignTemp(this);

  cout << "\tmovl\t" << _expr << ", %eax" << endl;

  if(_type.size() != SIZEOF_CHAR){
    cout << "\tmovl\t(%eax), %eax" << endl;
  }else {
    cout << "\tmovsbl\t(%eax), %eax" << endl;
  }

  cout << "\tmovl\t%eax, " << _operand << endl;
}

void Dereference::generate(bool &indirect) {
  indirect = true;
  _expr->generate();
  _operand = _expr->_operand;
}

void Address::generate() {
  bool indirect;
  _expr->generate(indirect);
  if(indirect){
    _operand = _expr->_operand;
  }else{
    assignTemp(this);
    cout << "\tleal\t" << _expr << ", %eax" << endl;
    cout << "\tmovl\t%eax, " << _operand << endl;
  }
}

void Promote::generate() {
    _expr->generate();
    assignTemp(this);
    cout << "\tmovb\t" << _expr << ", %al" << endl;
  	cout << "\tmovsbl\t%al, %eax" << endl;
  	cout << "\tmovl\t%eax, " << this << endl;
}

void Return::generate() {
    _expr->generate();
    cout << "\tmovl\t" << _expr << ", %eax" << endl;
    cout << "\tjmp\t" << *retLbl<< endl;
}

//------------ Break Statement ------------- //

void Break::generate(){
	cout << "\tjmp\t\t" << loop_labels.back() << endl;
}

void If::generate() {
  _expr->generate();
  Label if_label, else_label;

  cout << "\tmovl\t" << _expr << ", %eax" << endl;
  cout << "\tcmpl\t$0, %eax" << endl;
  cout << "\tje\t\t" << if_label << endl;

  _thenStmt->generate();
  if( _elseStmt != NULL ){
    cout << "\tjmp\t\t" << else_label << endl;
    cout << if_label << ":" << endl;
    _elseStmt->generate();
  }else {
    cout << if_label << ":" << endl;
  }
  cout << else_label << ":" << endl;
}

void For::generate() {
  Label for_label, exit_for_label;
  loop_labels.push_back(exit_for_label);

  _init->generate();
  cout << for_label << ":" << endl;

  _expr->generate();
  cout << "\tmovl\t" << _expr << ", %eax" << endl;
  cout << "\tcmpl\t$0, %eax" << endl;
  cout << "\tje\t\t" << exit_for_label << endl;

  _stmt->generate();
  _incr->generate();
  cout << "\tjmp\t\t" << for_label << endl;
  cout << exit_for_label << ":" << endl;
  loop_labels.pop_back();
}

void While::generate() {
  Label while_label, exit_while_label;
  loop_labels.push_back(exit_while_label);

  cout << while_label << ":" << endl;
  _expr->generate();
  cout << "\tmovl\t" << _expr << ", %eax" << endl;
  cout << "\tcmpl\t$0, %eax" << endl;
  cout << "\tje\t\t" << exit_while_label << endl;

  _stmt->generate();
  cout << "\tjmp\t\t" << while_label << endl;
  cout << exit_while_label << ":" << endl;
  loop_labels.pop_back();
}

void String::generate() {
    Label stringLabel;
    stringstream ss;
    ss << stringLabel;
    labelBuff.push_back(ss.str() +":\t.asciz\t" + _value);

    _operand = ss.str();
}
