/*
 * File:	allocator.cpp
 *
 * Description:	This file contains the member function definitions for
 *		functions dealing with storage allocation.  The actual
 *		classes are declared elsewhere, mainly in Tree.h.
 *
 *		Extra functionality:
 *		- maintaining minimum offset in nested blocks
 *		- allocation of structure types
 *		- allocation within statements
 */

# include <map>
# include <cassert>
# include <iostream>
# include "checker.h"
# include "machine.h"
# include "Tree.h"

using namespace std;

static map<string,unsigned>sizes;


/*
 * Function:	Type::size
 *
 * Description:	Return the size of a type in bytes.
 */

unsigned Type::size() const
{
    unsigned count;


    assert(_kind != FUNCTION && _kind != ERROR);
    count = (_kind == ARRAY ? _length : 1);

    if (_indirection > 0 || _kind == CALLBACK)
	return count * SIZEOF_PTR;

    if (_specifier == "char")
	return count * SIZEOF_CHAR;

    if (_specifier == "int")
	return count * SIZEOF_INT;

    if (sizes.count(_specifier) > 0)
	return count * sizes[_specifier];


    /* The size of a structure is the size of all of its fields, but with
       each field aligned and the entire structure aligned as well.  Since
       this is rather expensive to compute, we cache the result. */

    unsigned align, size = 0;
    const Symbols &symbols = getFields(_specifier)->symbols();

    for (unsigned i = 0; i < symbols.size(); i ++) {
	align = symbols[i]->type().alignment();

	if (size % align != 0)
	    size += (align - size % align);

	symbols[i]->_offset = size;
	size += symbols[i]->type().size();
    }

    align = alignment();

    if (size % align != 0)
	size += (align - size % align);

    sizes[_specifier] = size;
    return count * size;
}


/*
 * Function:	Type::alignment
 *
 * Description:	Return the alignment of a type in bytes.
 */

unsigned Type::alignment() const
{
    assert(_kind != FUNCTION && _kind != ERROR);

    if (_indirection > 0 || _kind == CALLBACK)
	return ALIGNOF_PTR;

    if (_specifier == "char")
	return ALIGNOF_CHAR;

    if (_specifier == "int")
	return ALIGNOF_INT;


    /* The alignment of a structure is the maximum alignment of its fields. */

    unsigned align = 0;
    const Symbols &symbols = getFields(_specifier)->symbols();

    for (unsigned i = 0; i < symbols.size(); i ++)
	if (symbols[i]->type().alignment() > align)
	    align = symbols[i]->type().alignment();

    return align;
}


/*
 * Function:	Block::allocate
 *
 * Description:	Allocate storage for this block.  We assign decreasing
 *		offsets for all symbols declared within this block, and
 *		then for all symbols declared within any nested block.
 *		Only symbols that have not already been allocated an offset
 *		will be assigned one, since the parameters are already
 *		assigned special offsets.
 */

void Block::allocate(int &offset) const
{
    int temp, saved;
    const Symbols &symbols = _decls->symbols();


    for (auto symbol : symbols)
	if (symbol->_offset == 0) {
	    offset -= symbol->type().size();
	    symbol->_offset = offset;
	}

    saved = offset;

    for (auto stmt : _stmts) {
	temp = saved;
	stmt->allocate(temp);
	offset = min(offset, temp);
    }
}


/*
 * Function:	While::allocate
 *
 * Description:	Allocate storage for this while statement, which
 *		essentially means allocating storage for variables declared
 *		as part of its statement.
 */

void While::allocate(int &offset) const
{
    _stmt->allocate(offset);
}


/*
 * Function:	For::allocate
 *
 * Description:	Allocate storage for this for statement, which
 *		essentially means allocating storage for variables declared
 *		as part of its statement.
 */

void For::allocate(int &offset) const
{
    _stmt->allocate(offset);
}


/*
 * Function:	If::allocate
 *
 * Description:	Allocate storage for this if-then or if-then-else
 *		statement, which essentially means allocating storage for
 *		variables declared as part of its statements.
 */

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


/*
 * Function:	Procedure::allocate
 *
 * Description:	Allocate storage for this function and return the number of
 *		bytes required.  The parameters are allocated offsets as
 *		well, starting with the given offset.
 */

void Procedure::allocate(int &offset) const
{
    Parameters *params = _id->type().parameters();
    const Symbols &symbols = _body->declarations()->symbols();

    for (unsigned i = 0; i < params->size(); i ++) {
	symbols[i]->_offset = offset;
	offset += (*params)[i].promote().size();
    }

    offset = 0;
    _body->allocate(offset);
}
