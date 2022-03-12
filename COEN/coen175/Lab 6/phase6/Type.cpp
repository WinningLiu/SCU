/*
 * File:	Type.cpp
 *
 * Description:	This file contains the member function definitions for
 *		types in Simple C.  A type is either a scalar type, an
 *		array type, a function type, or a callback.
 *
 *		Note that we simply don't like putting function definitions
 *		in the header file.  The header file is for the interface.
 *		Actually, we prefer opaque pointer types in C where you
 *		don't even get to see what's inside, much less touch it.
 *		But, C++ lets us have value types with access control
 *		instead of just always using pointer types.
 *
 *		Extra functionality:
 *		- equality and inequality operators
 *		- predicate functions such as isArray()
 *		- stream operator
 *		- the error type
 */

# include <cassert>
# include "Type.h"

using namespace std;


/*
 * Function:	Type::Type (constructor)
 *
 * Description:	Initialize this type as an error type.
 */

Type::Type()
    : Type(ERROR, "-error-", 0)
{
}


/*
 * Function:	Type::Type (constructor)
 *
 * Description:	Initialize this type object.
 */

Type::Type(int kind, const string &specifier, unsigned indirection)
    : _kind(kind), _specifier(specifier), _indirection(indirection)
{
    _parameters = nullptr;
}


/*
 * Function:	Type::operator ==
 *
 * Description:	Return whether another type is equal to this type.  The
 *		parameter lists are checked for function types, which C++
 *		makes so easy.  (At least, it makes something easy!)
 */

bool Type::operator ==(const Type &rhs) const
{
    if (_kind != rhs._kind)
	return false;

    if (_kind == ERROR)
	return true;

    if (_specifier != rhs._specifier)
	return false;

    if (_indirection != rhs._indirection)
	return false;

    if (_kind == SCALAR)
	return true;

    if (_kind == ARRAY)
	return _length == rhs._length;

    if (!_parameters || !rhs._parameters)
	return true;

    return *_parameters == *rhs._parameters;
}


/*
 * Function:	Type::operator !=
 *
 * Description:	Well, what do you think it does?  Why can't the language
 *		generate this function for us?  Because they think we want
 *		it to do something else?  Yeah, like that'd be a good idea.
 */

bool Type::operator !=(const Type &rhs) const
{
    return !operator ==(rhs);
}


/*
 * Function:	Type::isArray
 *
 * Description:	Return whether this type is an array type.
 */

bool Type::isArray() const
{
    return _kind == ARRAY;
}


/*
 * Function:	Type::isScalar
 *
 * Description:	Return whether this type is a scalar type.
 */

bool Type::isScalar() const
{
    return _kind == SCALAR;
}


/*
 * Function:	Type::isCallback
 *
 * Description:	Return whether this type is a callback type.
 */

bool Type::isCallback() const
{
    return _kind == CALLBACK;
}


/*
 * Function:	Type::isFunction
 *
 * Description:	Return whether this type is a function type.
 */

bool Type::isFunction() const
{
    return _kind == FUNCTION;
}


/*
 * Function:	Type::isError
 *
 * Description:	Return whether this type is an error type.
 */

bool Type::isError() const
{
    return _kind == ERROR;
}


/*
 * Function:	Type::specifier (accessor)
 *
 * Description:	Return the specifier of this type.
 */

const string &Type::specifier() const
{
    return _specifier;
}


/*
 * Function:	Type::indirection (accessor)
 *
 * Description:	Return the number of levels of indirection of this type.
 */

unsigned Type::indirection() const
{
    return _indirection;
}


/*
 * Function:	Type::length (accessor)
 *
 * Description:	Return the length of this type, which must be an array
 *		type.  Is there a better way than calling assert?  There
 *		certainly isn't an easier way.
 */

unsigned Type::length() const
{
    assert(_kind == ARRAY);
    return _length;
}


/*
 * Function:	Type::parameters (accessor)
 *
 * Description:	Return the parameters of this type, which must be a
 *		function or callback type.
 */

Parameters *Type::parameters() const
{
    assert(_kind == FUNCTION || _kind == CALLBACK);
    return _parameters;
}


/*
 * Function:	Type::isStruct
 *
 * Description:	Return whether this type has a struct specifier.
 */

bool Type::isStruct() const
{
    return _kind != ERROR && _specifier != "int" && _specifier != "char";
}


/*
 * Function:	Type::isValue
 *
 * Description:	Check if this type is a value type after any promotion.
 */

bool Type::isValue() const
{
    return !(_kind == SCALAR && _indirection == 0 && isStruct());
}


/*
 * Function:	Type::isInteger
 *
 * Description:	Check if this type is the integer type after any promotion.
 */

bool Type::isInteger() const
{
    return _kind == SCALAR && _indirection == 0 && !isStruct();
}


/*
 * Function:	Type::isPointer
 *
 * Description:	Check if this type is a pointer type after any promotion.
 */

bool Type::isPointer() const
{
    return (_kind == SCALAR && _indirection > 0) || _kind == ARRAY;
}


/*
 * Function:	Type::isCompatibleWith
 *
 * Description:	Check if this type is compatible with the other given type.
 *		In Simple C, two types are compatible if they are identical
 *		value types after promotion.
 */

bool Type::isCompatibleWith(const Type &that) const
{
    return isValue() && promote() == that.promote();
}


/*
 * Function:	Type::promote
 *
 * Description:	Return the result of performing type promotion on this
 *		type.  In Simple C, a characcter is promoted to an integer,
 *		an array is promoted to a pointer, and a function is
 *		promoted to a callback.
 */

Type Type::promote() const
{
    if (_kind == FUNCTION)
	return Callback(_specifier, _indirection);

    if (_kind == ARRAY)
	return Scalar(_specifier, _indirection + 1);

    if (_kind == SCALAR && _indirection == 0 && _specifier == "char")
	return Scalar("int");

    return *this;
}


/*
 * Function:	Type::deref
 *
 * Description:	Return the result of dereferencing this type, which must be
 *		a pointer type.
 */

Type Type::deref() const
{
    assert(_kind == SCALAR && _indirection > 0);
    return Scalar(_specifier, _indirection - 1);
}


/*
 * Function:	Scalar::Scalar (constructor)
 *
 * Description:	Initialize a scalar type object.
 */

Scalar::Scalar(const string &specifier, unsigned indirection)
    : Type(SCALAR, specifier, indirection)
{
}


/*
 * Function:	Array::Array
 *
 * Description:	Initialize an array type object.
 */

Array::Array(const string &specifier, unsigned indirection, unsigned length)
    : Type(ARRAY, specifier, indirection)
{
    _length = length;
}


/*
 * Function:	Function::Function
 *
 * Description:	Initialize a function type object.
 */

Function::Function(const string &specifier, unsigned indirection, Parameters *params)
    : Type(FUNCTION, specifier, indirection)
{
    _parameters = params;
}


/*
 * Function:	Callback::Callback
 *
 * Description:	Initialize a callback type object.
 */

Callback::Callback(const string &specifier, unsigned indirection)
    : Type(CALLBACK, specifier, indirection)
{
}


/*
 * Function:	operator <<
 *
 * Description:	Write a type to the specified output stream.  At least C++
 *		let's us do some cool things.
 */

ostream &operator <<(ostream &ostr, const Type &type)
{
    ostr << type.specifier();

    if (type.indirection() > 0)
	ostr << " " << string(type.indirection(), '*');

    if (type.isArray())
	ostr << "[" << type.length() << "]";

    else if (type.isFunction())
	ostr << "()";

    else if (type.isCallback())
	ostr << "(*)()";

    return ostr;
}
