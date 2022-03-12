/*
 * File:	generator.cpp
 *
 * Description:	This file contains the public and member function
 *		definitions for the code generator for Simple C.
 *
 *		Extra functionality:
 *		- putting all the global declarations at the end
 */

# include <cassert>
# include <iostream>
# include <map>
# include "generator.h"
# include "machine.h"
# include "Tree.h"
# include "Label.h"
# include "string.h"

using namespace std;

static int offset;
static string funcname;
static ostream &operator <<(ostream &ostr, Expression *expr);

static Register *eax = new Register("%eax", "%al");
static Register *ecx = new Register("%ecx", "%cl");
static Register *edx = new Register("%edx", "%dl");

static vector<Register *> registers = {eax, ecx, edx};

static map<string, Label> strings;

static void findBaseAndOffset(Expression *expr, Expression *&base, int &offset);


/* These will be replaced with functions in the next phase.  They are here
   as placeholders so that Call::generate() is finished. */


/*
 * Function:	align (private)
 *
 * Description:	Return the number of bytes necessary to align the given
 *		offset on the stack.
 */

static int align(int offset)
{
    if (offset % STACK_ALIGNMENT == 0)
	return 0;

    return STACK_ALIGNMENT - (abs(offset) % STACK_ALIGNMENT);
}


/*
 * Function:	operator << (private)
 *
 * Description:	Convenience function for writing the operand of an
 *		expression using the output stream operator.
 */

static ostream &operator <<(ostream &ostr, Expression *expr)
{
    if (expr->_register != nullptr)
	    return ostr << expr->_register;

    expr->operand(ostr);
    return ostr;
}

// Finished
void assign(Expression *expr, Register *reg) {
    if (expr != nullptr) {
        if (expr->_register != nullptr)
            expr->_register->_node = nullptr;
        expr->_register = reg;
    }
    if (reg != nullptr) {
        if (reg->_node != nullptr)
            reg->_node->_register = nullptr;
        reg->_node = expr;
    }
}

// Finished
void load(Expression *expr, Register *reg) {
    if (reg->_node != expr) {
        if (reg->_node != nullptr) {
            offset = offset - reg->_node->type().size();
            reg->_node->_offset = offset;
            cout << (reg->_node->type().size() == 1 ? "\tmovb\t" : "\tmovl\t");
            cout << reg << ", " << offset << "(%ebp)" << endl;
        }
        if (expr != nullptr) {
            if (expr->type().size() == 1)
                cout << "\tmovb\t" << expr << ", " << reg->name(expr->type().size()) << endl;
            else
                cout << "\tmovl\t" << expr << ", " << reg->name(expr->type().size()) << endl;
        }
        assign(expr, reg);
    }
}

// Finished
Register *getReg() {
    for (auto r : registers) {
        if (r->_node == nullptr)
            return r;
    }
    load(nullptr, registers[0]);
    return registers[0];
}


/*
 * Function:	Expression::operand
 *
 * Description:	Write an expression as an operand to the specified stream.
 */

void Expression::operand(ostream &ostr) const
{
    assert(_offset != 0);
    ostr << _offset << "(%ebp)";
}


/*
 * Function:	Identifier::operand
 *
 * Description:	Write an identifier as an operand to the specified stream.
 */

void Identifier::operand(ostream &ostr) const
{
    if (_symbol->_offset == 0)
	ostr << global_prefix << _symbol->name();
    else
	ostr << _symbol->_offset << "(%ebp)";
}


/*
 * Function:	Number::operand
 *
 * Description:	Write a number as an operand to the specified stream.
 */

void Number::operand(ostream &ostr) const
{
    ostr << "$" << _value;
}


/*
 * Function:	Call::generate
 *
 * Description:	Generate code for a function call expression.
 *
 * 		On a 32-bit Linux platform, the stack needs to be aligned
 * 		on a 4-byte boundary.  (Online documentation seems to
 * 		suggest that 16 bytes are required, but 4 bytes seems to
 * 		work and is much easier.)  Since all arguments are 4-bytes
 *		wide, the stack will always be aligned.
 *
 *		On a 32-bit OS X platform, the stack needs to aligned on a
 *		16-byte boundary.  So, if the stack will not be aligned
 *		after pushing the arguments, we first adjust the stack
 *		pointer.  However, this trick only works if none of the
 *		arguments are themselves function calls.
 *
 *		To handle nested function calls, we need to generate code
 *		for the nested calls first, which requires us to save their
 *		results and then push them on the stack later.  For
 *		efficiency, we only first generate code for the nested
 *		calls, but generate code for ordinary arguments in place.
 */

void Call::generate()
{
    unsigned numBytes;


    /* Generate code for any nested function calls first. */

    numBytes = 0;

    for (int i = _args.size() - 1; i >= 0; i --) {
	numBytes += _args[i]->type().size();

	if (STACK_ALIGNMENT != SIZEOF_REG && _args[i]->_hasCall)
	    _args[i]->generate();
    }


    /* Align the stack if necessary. */

    if (align(numBytes) != 0) {
	cout << "\tsubl\t$" << align(numBytes) << ", %esp" << endl;
	numBytes += align(numBytes);
    }


    /* Generate code for any remaining arguments and push them on the stack. */

    for (int i = _args.size() - 1; i >= 0; i --) {
	if (STACK_ALIGNMENT == SIZEOF_REG || !_args[i]->_hasCall)
	    _args[i]->generate();

	cout << "\tpushl\t" << _args[i] << endl;
	assign(_args[i], nullptr);
    }


    /* Call the function and then reclaim the stack space. */

    load(nullptr, eax);
    load(nullptr, ecx);
    load(nullptr, edx);

    if (_expr->type().isCallback()) {
	_expr->generate();

	if (_expr->_register == nullptr)
	    load(_expr, getReg());

	cout << "\tcall\t*" << _expr << endl;
	assign(_expr, nullptr);

    } else
	cout << "\tcall\t" << _expr << endl;

    if (numBytes > 0)
	cout << "\taddl\t$" << numBytes << ", %esp" << endl;

    assign(this, eax);
}


/*
 * Function:	Block::generate
 *
 * Description:	Generate code for this block, which simply means we
 *		generate code for each statement within the block.
 */

void Block::generate()
{
    for (auto stmt : _stmts) {
	stmt->generate();

	for (auto reg : registers)
	    assert(reg->_node == nullptr);
    }
}


/*
 * Function:	Simple::generate
 *
 * Description:	Generate code for a simple (expression) statement, which
 *		means simply generating code for the expression.
 */

void Simple::generate()
{
    _expr->generate();
    assign(_expr, nullptr);
}


/*
 * Function:	Procedure::generate
 *
 * Description:	Generate code for this function, which entails allocating
 *		space for local variables, then emitting our prologue, the
 *		body of the function, and the epilogue.
 */

void Procedure::generate()
{
    int param_offset;


    /* Assign offsets to the parameters and local variables. */

    param_offset = 2 * SIZEOF_REG;
    offset = param_offset;
    allocate(offset);


    /* Generate our prologue. */

    funcname = _id->name();
    cout << global_prefix << funcname << ":" << endl;
    cout << "\tpushl\t%ebp" << endl;
    cout << "\tmovl\t%esp, %ebp" << endl;
    cout << "\tsubl\t$" << funcname << ".size, %esp" << endl;


    /* Generate the body of this function. */

    _body->generate();


    /* Generate our epilogue. */

    cout << endl << global_prefix << funcname << ".exit:" << endl;
    cout << "\tmovl\t%ebp, %esp" << endl;
    cout << "\tpopl\t%ebp" << endl;
    cout << "\tret" << endl << endl;

    offset -= align(offset - param_offset);
    cout << "\t.set\t" << funcname << ".size, " << -offset << endl;
    cout << "\t.globl\t" << global_prefix << funcname << endl << endl;
}


/*
 * Function:	generateGlobals
 *
 * Description:	Generate code for any global variable declarations.
 */

void generateGlobals(Scope *scope)
{
    const Symbols &symbols = scope->symbols();

    for (auto symbol : symbols)
	if (!symbol->type().isFunction()) {
	    cout << "\t.comm\t" << global_prefix << symbol->name() << ", ";
	    cout << symbol->type().size() << endl;
	}

    cout << "\t.data" << endl;
    for (const auto str : strings) 
        cout << str.second << ":\t.asciz\t\"" << escapeString(str.first) << "\"" << endl;
}


/*
 * Function:	Assignment::generate
 *
 * Description:	Generate code for an assignment statement.
 *
 *		NOT FINISHED: Only works if the right-hand side is an
 *		integer literal and the left-hand side is an integer
 *		scalar.
 */

// Finished
void Assignment::generate()
{
    int offset;
    Expression *base, *ptr;
    unsigned num;
    _right->generate();

    if (_right->_register == nullptr && _right->isNumber(num) == false) 
        load(_right, getReg());

    findBaseAndOffset(_left, base, offset);

    if (base->isDereference(ptr)) {
        ptr->generate();
        if (ptr->_register == nullptr)
            load(ptr, getReg());
        cout << (_right->type().size() == 1 ? "\tmovb\t" : "\tmovl\t") << _right << ", " << offset << "(" << ptr << ")" << endl;
        assign(ptr, nullptr);
    }
    else 
        cout << (_right->type().size() == 1 ? "\tmovb\t" : "\tmovl\t") << _right << ", " << offset << "+" << base << endl;
    assign(_right, nullptr);
}

// Finished
static void compute(Expression *result, Expression *left, Expression *right, const string &opcode) {
    left->generate();
    right->generate();

    if (left->_register == nullptr)
        load(left, getReg());

    cout << "\t" << opcode << "\t" << right << ", " << left << endl;

    assign(right, nullptr);
    assign(result, left->_register);
}

void Add::generate() {
    compute(this, _left, _right, "addl");
}

void Subtract::generate() {
    compute(this, _left, _right, "subl");
}

void Multiply::generate() {
    compute(this, _left, _right, "imull");
}

// Finished
static void divide(Expression *result, Expression *left, Expression *right, Register *reg) {
    left->generate();
    right->generate();

    load(left, eax);
    load(nullptr, edx);

    unsigned num;
    if (right->isNumber(num))
        load(right, ecx);

    cout << "\tcltd\t" << endl;
    cout << "\tidivl\t" << right << endl;

    assign(nullptr, eax);
    assign(nullptr, ecx);
    assign(nullptr, edx);
    assign(result, reg);
}

void Divide::generate() {
    divide(this, _left, _right, eax);
}

void Remainder::generate() {
    divide(this, _left, _right, edx);
}

// Finished
static void compare(Expression *result, Expression *left, Expression *right, const string &opcode) {
    left->generate();
    right->generate();

    if (left->_register == nullptr)
        load(left, eax);
    
    cout << "\tcmpl\t" << right << ", " << left << endl;

    assign(result, left->_register);
    assign(left, nullptr);
    assign(right, nullptr);

    string size = result->_register->byte();

    cout << "\t" << opcode << "\t" << size << endl;
    cout << "\tmovzbl\t" << size << ", " << result << endl;
}

void LessThan::generate() {
    compare(this, _left, _right, "setl");
}

void GreaterThan::generate() {
    compare(this, _left, _right, "setg");
}

void LessOrEqual::generate() {
    compare(this, _left, _right, "setle");
}

void GreaterOrEqual::generate() {
    compare(this, _left, _right, "setge");
}

void Equal::generate() {
    compare(this, _left, _right, "sete");
}

void NotEqual::generate() {
    compare(this, _left, _right, "setne");
}

// Finished
void Cast::generate() {
    _expr->generate();
    
    if (_expr->_register == nullptr)
        load(_expr, getReg());

    //check if _expr is char and this is int (size)
    if (_expr->type().size() == 1 && this->type().size() == 4)
        cout << "\tmovsbl\t" << _expr << ", " << _expr->_register << endl;
    assign(this, _expr->_register);
    assign(_expr, nullptr);
}

// Finished
void Not::generate() {
    _expr->generate();
    if (_expr->_register == nullptr)
        load(_expr, getReg());

    cout << "\tcmpl\t$0, " << _expr << endl;

    string size = _expr->_register->byte();

    cout << "\tsete\t" << size << endl;
    cout << "\tmovzbl\t" << size << ", " << _expr << endl;

    assign(this, _expr->_register);
    assign(_expr, nullptr);
}

// Finished
void Negate::generate() {
    _expr->generate();
    if (_expr->_register == nullptr)
        load(_expr, getReg());
    cout << "\tnegl\t" << _expr << endl;

    assign(this, _expr->_register);
    assign(_expr, nullptr);
}

// Finished
void Dereference::generate() {
    _expr->generate();
    if (_expr->_register == nullptr)
        load(_expr, getReg());

    cout << (_expr->type().size() == 1 ? "\tmovb\t(" : "\tmovl\t(") << _expr << "), " << _expr << endl;

    assign(this, _expr->_register);
    assign(_expr, nullptr);
}

// Finished
void Address::generate() {
    Expression *base, *ptr; 
    int offset;

    findBaseAndOffset(_expr, base, offset);

    if (base->isDereference(ptr)) {
        ptr->generate();
        if (ptr->_register == nullptr)
            load(ptr, getReg());
        assign(this, ptr->_register);
        if (offset != 0)
            cout << "\taddl\t$" << offset << ", " << this << endl;
    }
    else {
        assign(this, getReg());
        cout << "\tleal\t" << offset << "+" << base << ", " << this << endl;
    }
}

// Finished
void String::operand(ostream &ostr) const {
    Label label;
    if (strings.find(_value) == strings.end()) 
        strings.insert(pair<string, Label> (_value, label));
    else
        label = strings[_value];
    ostr << label;
}

// Given
static void findBaseAndOffset(Expression *expr, Expression *&base, int &offset) {
    int field;

    base = expr;
    offset = 0;

    while (base->isField(base, field))
        offset += field;
}

// Finished
void Field::generate() {
    int offset;
    Expression *base, *ptr;

    findBaseAndOffset(this, base, offset);

    if (base->isDereference(ptr)) {
        ptr->generate();
        if (ptr->_register == nullptr)
            load(ptr, getReg());
        cout << (_type.size() == 1 ? "\tmovb\t" : "\tmovl\t") << offset << "(" << ptr << ")" << ", ";
        assign(this, ptr->_register);
        cout << this << endl;
        assign(ptr, nullptr);
    }
    else {
        assign(this, getReg());
        cout << (_type.size() == 1 ? "\tmovb\t" : "\tmovl\t") << offset << "+" << base << ", " << this << endl;
    }
}

// Finished
void LogicalAnd::generate() {
    Label shortC, skip;
    _left->test(shortC, false);
    _right->test(shortC, false);

    assign(this, getReg());

    cout << "\tmovl\t$1, " << this << endl;
    cout << "\tjmp\t" << skip << endl;
    cout << shortC << ":" << endl;
    cout << "\tmovl\t$0, " << this << endl;
    cout << skip << ":" << endl;
}

// Finished
void LogicalOr::generate() {
    Label shortC, skip;
    _left->test(shortC, true);
    _right->test(shortC, true);

    assign(this, getReg());

    cout << "\tmovl\t$0, " << this << endl;
    cout << "\tjmp\t" << skip << endl;
    cout << shortC << ":" << endl;
    cout << "\tmovl\t$1, " << this << endl;
    cout << skip << ":" << endl;
}

// Given
void Expression::test(const Label &label, bool ifTrue) {
    generate();

    if (_register == nullptr)
        load(this, getReg());

    cout << "\tcmpl\t$0, " << this << endl;
    cout << (ifTrue ? "\tjne\t" : "\tje\t") << label << endl;

    assign(this, nullptr);
}

// Finished
void Return::generate() {
    _expr->generate();
    load(_expr, eax);
    cout << "\tjmp\t" << funcname << ".exit" << endl;
    assign(_expr, nullptr);
    assign(nullptr, eax);
}

// Finished
void If::generate() {
    Label label, e; 

    _expr->generate();
    _expr->test(label, false);
    _thenStmt->generate();

    if (_elseStmt != nullptr) {
        cout << "\tjmp\t" << e << endl;
        cout << label << ":" << endl;
        _elseStmt->generate();
        cout << e << ":" << endl;
    }
    else 
        cout << label << ":" << endl;

    assign(_expr, nullptr);
}

// Finished
void While::generate() {
    Label label, e;

    cout << label << ":" << endl;
    _expr->test(e, false);
    _stmt->generate();

    cout << "\tjmp\t" << label << endl;
    cout << e << ":" << endl;
}

// Finished
void For::generate() {
    Label label, e;

    _init->generate();
    cout << label << ":" << endl;

    _expr->test(e, false);
    _stmt->generate();
    _incr->generate();

    cout << "\tjmp\t" << label << endl;
    cout << e << ":" << endl;
}