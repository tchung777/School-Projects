#include "Tree.h"
#include <iostream>
#include <sstream>

using namespace std;
static unsigned maxargs;
static int temp_offset;

string getTemp() {
  stringstream ss;
  temp_offset -= 4;
  ss << temp_offset << "(%ebp)";
  return ss.str();
}

ostream &operator <<(ostream &ostr, Expression *expr)
{
    return ostr << expr->_operand;
}

static vector<string> labelBuff;
static int funcLabelIdx = 0;
string currentFuncLabel;
string newFuncLabel() {
    stringstream ss;
    ss << ".F" << funcLabelIdx;
    funcLabelIdx++;
    currentFuncLabel = ss.str();
    return currentFuncLabel;
}

static int jumpLabelIdx = 0;
string currentJumpLabel;
string newJumpLabel() {
    stringstream ss;
    ss << ".L" << jumpLabelIdx;
    jumpLabelIdx++;
    currentJumpLabel = ss.str();
    return currentJumpLabel;
}

static int whileLabelIdx = 0;
string currentWhileLabel;
string newWhileLabel() {
    stringstream ss;
    ss << ".WHILE" << whileLabelIdx;
    whileLabelIdx++;
    currentWhileLabel = ss.str();
    return currentWhileLabel;
}

static int forLabelIdx = 0;
string currentForLabel;
string newForLabel() {
    stringstream ss;
    ss << ".FOR" << forLabelIdx;
    forLabelIdx++;
    currentForLabel = ss.str();
    return currentForLabel;
}

static int andLabelIdx = 0;
string currentAndLabel;
string newAndLabel() {
    stringstream ss;
    ss << ".AND" << andLabelIdx;
    andLabelIdx++;
    currentAndLabel = ss.str();
    return currentAndLabel;
}

static int orLabelIdx = 0;
string currentOrLabel;
string newOrLabel() {
    stringstream ss;
    ss << ".OR" << orLabelIdx;
    orLabelIdx++;
    currentOrLabel = ss.str();
    return currentOrLabel;
}

static int ifLabelIdx = 0;
string currentIfLabel;
string newIfLabel() {
    stringstream ss;
    ss << ".IF" << ifLabelIdx;
    ifLabelIdx++;
    currentIfLabel = ss.str();
    return currentIfLabel;
}

void LogicalAnd::generate() {
    _left->generate();
    _operand = getTemp();

    string lval = _left->_operand;
    string rval;
    string andTrueLabel = newAndLabel();
    string andFalseLabel = newAndLabel();

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
    _operand = getTemp();

    string lval = _left->_operand;
    string rval;
    string orTrueLabel = newOrLabel();
    string orFalseLabel = newOrLabel();

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
    _operand = getTemp();

    cout << "\tmovl\t" << _left << ", %eax" <<endl;
    cout << "\tcmpl\t" << _right << ", %eax" << endl;

    cout << "\tsetl\t%al" << endl;
    cout << "\tmovzbl\t%al, %eax" << endl;
    cout << "\tmovl\t%eax, " << _operand << endl;
}

void LessOrEqual::generate() {
    _left->generate();
    _right->generate();
    _operand = getTemp();

    cout << "\tmovl\t" << _left << ", %eax" <<endl;
    cout << "\tcmpl\t" << _right << ", %eax" << endl;

    cout << "\tsetle\t%al" << endl;
    cout << "\tmovzbl\t%al, %eax" << endl;
    cout << "\tmovl\t%eax, " << _operand << endl;
}

void GreaterThan::generate() {
    _left->generate();
    _right->generate();
    _operand = getTemp();

    cout << "\tmovl\t" << _left << ", %eax" <<endl;
    cout << "\tcmpl\t" << _right << ", %eax" << endl;

    cout << "\tsetg\t%al" << endl;
    cout << "\tmovzbl\t%al, %eax" << endl;
    cout << "\tmovl\t%eax, " << _operand << endl;
}

void GreaterOrEqual::generate() {
    _left->generate();
    _right->generate();
    _operand = getTemp();

    cout << "\tmovl\t" << _left << ", %eax" <<endl;
    cout << "\tcmpl\t" << _right << ", %eax" << endl;

    cout << "\tsetge\t%al" << endl;
    cout << "\tmovzbl\t%al, %eax" << endl;
    cout << "\tmovl\t%eax, " << _operand << endl;
}

void Equal::generate() {
    _left->generate();
    _right->generate();
    _operand = getTemp();

    cout << "\tmovl\t" << _left << ", %eax" <<endl;
    cout << "\tcmpl\t" << _right << ", %eax" << endl;

    cout << "\tsete\t%al" << endl;
    cout << "\tmovzbl\t%al, %eax" << endl;
    cout << "\tmovl\t%eax, " << _operand << endl;
}

void NotEqual::generate() {
    _left->generate();
    _right->generate();
    _operand = getTemp();

    cout << "\tmovl\t" << _left << ", %eax" <<endl;
    cout << "\tcmpl\t" << _right << ", %eax" << endl;

    cout << "\tsetne\t%al" << endl;
    cout << "\tmovzbl\t%al, %eax" << endl;
    cout << "\tmovl\t%eax, " << _operand << endl;
}

void Add::generate() {
    _left->generate();
    _right->generate();
    _operand = getTemp();

    cout << "\tmovl\t" << _left << ", %eax" << endl;
    cout << "\taddl\t" << _right << ", %eax" << endl;
    cout << "\tmovl\t%eax, " << _operand << endl;
}

void Subtract::generate() {
    _left->generate();
    _right->generate();
    _operand = getTemp();

    cout << "\tmovl\t" << _left << ", %eax" << endl;
    cout << "\tsubl\t" << _right << ", %eax" << endl;
    cout << "\tmovl\t%eax, " << _operand << endl;
}

void Multiply::generate() {
    _left->generate();
    _right->generate();
    _operand = getTemp();

    cout << "\tmovl\t" << _left << ", %eax" << endl;
    cout << "\timull\t" << _right << ", %eax" << endl;
    cout << "\tmovl\t%eax, " << _operand << endl;
}

void Divide::generate() {
    _left->generate();
    _right->generate();
    _operand = getTemp();

    cout << "\tmovl\t" << _left << ", %eax" << endl;
    cout << "\tmovl\t" << _right << ", %ecx" << endl;
    cout << "\tcltd" << endl;

    cout << "\tidivl\t%ecx" << endl;
    cout << "\tmovl\t%eax, " << _operand << endl;
}

void Remainder::generate() {
    _left->generate();
    _right->generate();
    _operand = getTemp();

    cout << "\tmovl\t" << _left << ", %eax" << endl;
    cout << "\tmovl\t" << _right << ", %ecx" << endl;
    cout << "\tcltd" << endl;

    cout << "\tidivl\t%ecx" << endl;
    cout << "\tmovl\t%edx, " << _operand << endl;
}

void Not::generate() {
    _expr->generate();
    _operand = getTemp();

    cout << "\tmovl\t" << _expr << ", %eax" << endl;
    cout << "\tcmpl\t$0, %eax" << endl;

    cout << "\tsete\t%al" << endl;
    cout << "\tmovzbl\t%al, %eax" << endl;
    cout << "\tmovl\t%eax, " << _operand << endl;
}

void Negate::generate() {
    _expr->generate();
    _operand = getTemp();

    if(_expr->_operand[0] == '$' ) {
        string number = _expr->_operand;
        number.erase(0, 1);
        istringstream numbuff(number);
        int value;
        numbuff >> value;
        stringstream ss;
        ss << "$" << -value;
        _operand = ss.str();
    } else {
        cout << "\tmovl\t" << _expr << ", %eax" << endl;
        cout << "\tnegl\t%eax" << endl;
        cout << "\tmovl\t%eax, " <<  _operand << endl;
    }
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

void String::generate() {
  string stringLabel = newJumpLabel();
  stringstream ss;
  ss << stringLabel << ":" << "\t.asciz\t" << _value << endl;
  labelBuff.push_back(ss.str());

  _operand = stringLabel;
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

void Dereference::generate(){
    _expr->generate();
    _operand = getTemp();

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
    _operand = getTemp();
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
        _operand = getTemp();

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
    cout << "\tjmp\t" << currentFuncLabel << endl;
}

void If::generate() {
    _expr->generate();
    string exprVal = _expr->_operand;
    string endLabel = newIfLabel();
    string elseLabel;

    if(exprVal[0] == '$') {
        cout << "\tmovl\t" << exprVal << ", %eax" << endl;
        exprVal = "%eax";
    }

    cout << "\tcmpl\t$0, " << exprVal << endl;

    if (_elseStmt != nullptr) {
        elseLabel = newIfLabel();

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
    string loopLabel = newForLabel();
    string condLabel = newForLabel();

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
    string loopLabel = newWhileLabel();
    string condLabel = newWhileLabel();

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

void Assignment::generate()
{
    bool indirect;
    _left->generate(indirect);
    _right->generate();

    int lLength = _left->type().size();

    if (indirect) {
        if (lLength == 1) {
            cout << "\tmovl\t" << _right << ", %eax" << endl;
            cout << "\tmovl\t" << _left->_operand << ", %ecx" << endl;
            cout << "\tmovb\t%al, (%ecx)" << endl;
        } else {
            cout << "\tmovl\t" << _right << ", %eax" << endl;
            cout << "\tmovl\t" << _left << ", %ecx" << endl;
            cout << "\tmovl\t%eax, (%ecx)" << endl;
        }
    } else {
        if (lLength == 1) {
            cout << "\tmovl\t" << _right << ", %eax" << endl;
            cout << "\tmovb\t%al, " << _left << endl;
        } else {
            cout << "\tmovl\t" << _right << ", %eax" << endl;
            cout << "\tmovl\t%eax, " << _left << endl;
        }
    }
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
