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
# include <vector>
# include "generator.h"
# include "machine.h"
# include "label.h"

using namespace std;

static unsigned maxargs;
int temp_offset;
Label* retLbl;
vector<string> strLbls;
vector<Label> loopLbls;


// Ex) _operand = "-24(%ebp)"
void assigntemp(Expression* expr)
{
    stringstream ss;
    temp_offset -= 4;  // decrement temp_offset
    ss << temp_offset << "(%ebp)";
    expr->_operand = ss.str();  // assign offset to _operand
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

/*
 * Function:	Call::generate
 *
 * Description:	Generate code for a function call expression, in which each
 *		argument is simply a variable or an integer literal.
 */

void Call::generate()
{
    unsigned numBytes = 0;
    assigntemp(this);


    for (int i = _args.size() - 1; i >= 0; i --) {
        _args[i]->generate();
        cout << "\tpushl\t" << _args[i] << endl;
        numBytes += _args[i]->type().size();
    }

    cout << "\tcall\t" << global_prefix << _id->name() << endl;

    if (numBytes > 0) {
        cout << "\taddl\t$" << numBytes << ", %esp" << endl;
    }

    cout << "\tmovl\t%eax, " << this << endl;
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
    if (_args.size() > maxargs) {
        maxargs = _args.size();
    }

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

// void Assignment::generate()
// {
//     _left->generate();
//     _right->generate();

//     cout << "\tmovl\t" << _right << ", %eax" << endl;
//     cout << "\tmovl\t%eax, " << _left << endl;
// }


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

    retLbl = new Label();

    /* Generate our prologue. */
    allocate(offset);
    cout << global_prefix << _id->name() << ":" << endl;
    cout << "\tpushl\t%ebp" << endl;
    cout << "\tmovl\t%esp, %ebp" << endl;
    cout << "\tsubl\t$" << _id->name() << ".size, %esp" << endl;


    /* Generate the body of this function. */
    maxargs = 0;
    temp_offset = offset;  // Save offset
    _body->generate();
    offset = temp_offset;  // Restore offset

    offset -= maxargs * SIZEOF_ARG;

    while ((offset - PARAM_OFFSET) % STACK_ALIGNMENT)
	offset --;


    /* Generate our epilogue. */
    cout << *retLbl << ":" << endl;  // Global return label to point to epilogue

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
    if (globals.size() > 0) {
        cout << "\t.data" << endl;
    }

    for (unsigned i = 0; i < globals.size(); i ++) {
        cout << "\t.comm\t" << global_prefix << globals[i]->name();
        cout << ", " << globals[i]->type().size();
        cout << ", " << globals[i]->type().alignment() << endl;
    }

    for (unsigned i = 0; i < strLbls.size(); i++) {
        cout << strLbls[i] << endl;
    }
}

void Expression::generate()
{
    std::cerr << "oops" << std::endl;  // This tells us there is more work to be done
}

void Expression::generate(bool& indirect)
{
    indirect = false;
    generate();
}

void Address::generate()
{
    bool indirect;
    _expr->generate(indirect);  // generate code for child

    if (indirect) {  // This is case (address of *p, which is p)
        _operand = _expr->_operand;
    } else {  // This is case (address of p)
        assigntemp(this);  // Given an expression pointer, put next temporary on the stack
        cout << "\tleal\t" << _expr << ", %eax" << endl;  // load effective address long
        cout << "\tmovl\t%eax, " << this << endl;
    }
}

void Dereference::generate(bool& indirect)
{
    indirect = true;
    _expr->generate();
    _operand = _expr->_operand;
}

void Dereference::generate()
{
    _expr->generate();  // generate code fo the child
    assigntemp(this);
    cout << "\tmovl\t" << _expr << ",%eax" << endl;

    if (_type.size() == 1) {  // are you a byte or word
        cout << "\tmovsbl\t(%eax), %eax" << endl;  // move sign extend byte to long
    } else {
        cout << "\tmovl\t(%eax), %eax" << endl;
    }
    cout << "\tmovl\t%eax," << this << endl;  // 0 offset

    // Move     movl    g, %eax
    // Op       movl    (%eax), %eax
    // Store    movl    %eax, temp
}

void Assignment::generate()
{
    bool indirect;
    _left->generate(indirect);
    _right->generate();  // always an rvalue
    // _left->generate(indirect);

    if (indirect) {  // *p = x
        cout << "\tmovl\t" << _right << ", %eax" << endl;  // right should always be a 4 byte quantity
        cout << "\tmovl\t" << _left << ", %ecx" << endl;  // left should be an address
        if (_left->type().size() == 1) {
            cout << "\tmovb\t%al, (%ecx)" << endl;  // %al is low byte of %eax (truncation)
        } else {
            cout << "\tmovl\t%eax, (%ecx)" << endl;
        }
    } else {  // y = x
        cout << "\tmovl\t" << _right << ",%eax" << endl;
        if (_left->type().size() == 1) {
            cout << "\tmovbl\t%al, " << _left << endl;
        } else {
            cout << "\tmovl\t%eax, " << _left << endl;
        }
    }

    // Note: only two ways to get lvalues in simple c
    //      - IDs
    //      - Dereferences
}

void String::generate()
{
    stringstream strlbl_ss, out;
    Label strLbl;  // Increment counter on a new label object
    string strLbl_str;

    strlbl_ss << strLbl;
    strLbl_str = strlbl_ss.str();  // Turn string's label to string type

    // `.L<number>` is printed out by the overloaded operator for label
    out << strLbl_str << ":\t.asciz\t" << _value;

    strLbls.push_back(out.str());

    _operand = strLbl_str;  // Set Operand
}

void Return::generate()
{
    _expr->generate();  // Generate code for expression of return statement

    cout << "\tmovl\t" << _expr << ", %eax" << endl;  // Store return value in %eax
    cout << "\tjmp\t" << *retLbl << endl;  // Jump to return label (epilogue)
}

void Add::generate()
{
    _left->generate();
    _right->generate();
    assigntemp(this);

    cout << "\tmovl\t" << _left << ", %eax" << endl;
    cout << "\taddl\t" << _right << ", %eax" << endl;
    cout << "\tmovl\t%eax,"<< this << endl;
}

void Subtract::generate()
{
    _left->generate();
    _right->generate();
    assigntemp(this);

    // left - right
    cout << "\tmovl\t" << _left << ", %eax" << endl;
    cout << "\tsubl\t" << _right << ", %eax" << endl;  // Subtracts _right FROM %eax
    cout << "\tmovl\t%eax, " << this << endl;
}

void Multiply::generate()
{
    _left->generate();
    _right->generate();
    assigntemp(this);

    cout << "\tmovl\t" << _left << ", %eax" << endl;
    cout << "\timull\t" << _right << ", %eax" << endl;
    cout << "\tmovl\t%eax, " << this << endl;
}

void Divide::generate()
{
    _left->generate();
    _right->generate();
    assigntemp(this);

    cout << "\tmovl\t" << _left << ", %eax" << endl;
    cout << "\tmovl\t" << _right << ", %ecx" << endl;
    cout << "\tcltd\t" << endl;  // convert signed long to signed double long. This sign extends %eax
    cout << "\tidivl\t%ecx" << endl;  // %edx:%eax diveded by %ecx. Quotient in %eax and Remainder in %edx
    cout << "\tmovl\t%eax, " << this << endl;
}

/**
 * This is the same as the divide function, but retrieves the remainder after the
 * divide function
*/
void Remainder::generate()
{
    _left->generate();
    _right->generate();
    assigntemp(this);

    cout << "\tmovl\t" << _left << ", %eax" << endl;
    cout << "\tmovl\t" << _right << ", %ecx" << endl;
    cout << "\tcltd\t" << endl;
    cout << "\tidivl\t%ecx" << endl;
    cout << "\tmovl\t%edx, " << this << endl;
}

void Negate::generate()
{
    _expr->generate();
    assigntemp(this);

    cout << "\tmovl\t" << _expr << ", %eax" << endl;
    cout << "\tnegl\t%eax" << endl;
    cout << "\tmovl\t%eax, " << this << endl;
}

void Not::generate()
{
    _expr->generate();
    assigntemp(this);

    cout << "\tmovl\t" << _expr << ", %eax" << endl;
    cout << "\tcmpl\t$0, %eax" << endl;
    cout << "\tsete\t%al" << endl;  // %al is the lower 8 bits
    cout << "\tmovzbl\t%al, %eax" << endl;  // Move zero-extended byte
    cout << "\tmovl\t%eax, " << this << endl;
}

void LessThan::generate()
{
    _left->generate();
    _right->generate();
    assigntemp(this);

    cout << "\tmovl\t" << _left << ", %eax" << endl;
    cout << "\tcmpl\t" << _right << ", %eax" << endl;
    cout << "\tsetl\t%al" << endl;  // set if less than
    cout << "\tmovzbl\t%al, %eax" << endl;
    cout << "\tmovl\t%eax, " << this << endl;
}

void GreaterThan::generate()
{
    _left->generate();
    _right->generate();
    assigntemp(this);

    cout << "\tmovl\t" << _left << ", %eax" << endl;
    cout << "\tcmpl\t" << _right << ", %eax" << endl;
    cout << "\tsetg\t%al" << endl;  // set if greater than
    cout << "\tmovzbl\t%al, %eax" << endl;
    cout << "\tmovl\t%eax, " << this << endl;
}

void LessOrEqual::generate()
{
    _left->generate();
    _right->generate();
    assigntemp(this);

    cout << "\tmovl\t" << _left << ", %eax" << endl;
    cout << "\tcmpl\t" << _right << ", %eax" << endl;
    cout << "\tsetle\t%al" << endl;  // set if less than equal to
    cout << "\tmovzbl\t%al, %eax" << endl;
    cout << "\tmovl\t%eax, " << this << endl;
}

void GreaterOrEqual::generate()
{
    _left->generate();
    _right->generate();
    assigntemp(this);

    cout << "\tmovl\t" << _left << ", %eax" << endl;
    cout << "\tcmpl\t" << _right << ", %eax" << endl;
    cout << "\tsetge\t%al" << endl;  // set if greater than equal to
    cout << "\tmovzbl\t%al, %eax" << endl;
    cout << "\tmovl\t%eax, " << this << endl;
}

void Equal::generate()
{
    _left->generate();
    _right->generate();
    assigntemp(this);

    cout << "\tmovl\t" << _left << ", %eax" << endl;
    cout << "\tcmpl\t" << _right << ", %eax" << endl;
    cout << "\tsete\t%al" << endl;  // set if equal to
    cout << "\tmovzbl\t%al, %eax" << endl;
    cout << "\tmovl\t%eax, " << this << endl;
}

void NotEqual::generate()
{
    _left->generate();
    _right->generate();
    assigntemp(this);

    cout << "\tmovl\t" << _left << ", %eax" << endl;
    cout << "\tcmpl\t" << _right << ", %eax" << endl;
    cout << "\tsetne\t%al" << endl;  // set if not equal to
    cout << "\tmovzbl\t%al, %eax" << endl;
    cout << "\tmovl\t%eax, " << this << endl;
}

void LogicalOr::generate()
{
    _left->generate();
    assigntemp(this);

    Label orlbl = Label();

    cout << "\tmovl\t" << _left << ",%eax" << endl;
    cout << "\tcmpl\t$0,%eax" << endl;
    cout << "\tjne\t" << orlbl << endl;

    _right->generate();

    cout << "\tmovl\t" << _right << ",%eax" << endl;
    cout << "\tcmpl\t$0,%eax" << endl;

    cout << orlbl << ":" << endl; 

    cout << "\tsetne\t%al" << endl;
    cout << "\tmovzbl\t%al,%eax" << endl;
    cout << "\tmovl\t%eax," << this << endl;
}

void LogicalAnd::generate()
{
    _left->generate();
    assigntemp(this);

    Label andlbl = Label();

    cout << "movl\t" << _left << ",%eax" << endl;
    cout << "cmpl\t$0,%eax" << endl;
    cout << "je\t" << andlbl << endl; 

    _right->generate();

    cout << "movl\t" << _right << ",%eax" << endl;
    cout << "cmpl\t$0,%eax" << endl;

    cout << andlbl << ":" << endl;

    cout << "\tsetne\t%al" << endl;
    cout << "\tmovzbl\t%al,%eax" << endl;
    cout << "\tmovl\t%eax," << this << endl;
}

void Promote::generate()
{
    _expr->generate();
    assigntemp(this);

    cout << "\tmovb\t" << _expr << ", %al" << endl;
    cout << "\tmovsbl\t%al, %eax" << endl;
    cout << "\tmovl\t%eax, " << this << endl;
}

void While::generate()
{
    Label whileLbl;
    cout << whileLbl << ":" << endl;

    _expr->generate();

    Label exitLbl;
    loopLbls.push_back(exitLbl);

    cout << "\tmovl\t" << _expr << ", %eax" << endl;
    cout << "\tcmpl\t$0, %eax" << endl;
    cout << "\tje\t" << exitLbl << endl;

    _stmt->generate();

    cout << "\tjmp\t" << whileLbl << endl;
    cout << exitLbl << ":" << endl;
    
    loopLbls.pop_back();
}

void For::generate()
{
    _init->generate();  // Code gen for statement

    Label forLbl;
    cout << forLbl << ":" << endl;

    _expr->generate();  // Code gen for expression

    Label exitLbl;
    loopLbls.push_back(exitLbl);

    cout << "\tmovl\t" << _expr << ", %eax" << endl;
    cout << "\tcmpl\t$0, %eax" << endl;
    cout << "\tje\t" << exitLbl << endl;

    _stmt->generate();

    _incr->generate();

    cout << "\tjmp\t" << forLbl << endl;
    cout << exitLbl << ":" << endl;

    loopLbls.pop_back();
}

void If::generate()
{
    _expr->generate();

    Label ifLbl;

    cout << "\tmovl\t" << _expr << ", %eax" << endl;
    cout << "\tcmpl\t$0, %eax" << endl;
    cout << "\tje\t" << ifLbl << endl;

    _thenStmt->generate();

    Label exitLbl;

    if (_elseStmt == NULL) {
        cout << ifLbl << ":" << endl;
    } else {
        cout << "\tjmp\t" << exitLbl << endl;
        cout << ifLbl << ":" << endl;
        _elseStmt->generate();
    }

    cout << exitLbl << ":" << endl;	
}

void Break::generate()
{
    cout << "\tjmp\t\t" << loopLbls.back() << endl;
}
