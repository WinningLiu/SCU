/*
 * File:	Type.h
 *
 * Description:	This file contains the class definition for types in Simple
 *		C.  A type is either a scalar type, an array type, a
 *		function type, or a callback.  These types include a
 *		specifier and the number of levels of indirection.  Array
 *		types also have a length, and function types also have a
 *		parameter list.  An error type is also supported for use in
 *		undeclared identifiers and the results of type checking.
 *
 *		By convention, a null parameter list represents an
 *		unspecified parameter list.  An empty parameter list is
 *		represented by an empty vector.
 *
 *		Subclassing is used to provide custom constructors that
 *		serve of factory functions.  However, all information is
 *		stored in the base class to avoid the problem of object
 *		slicing (since we'll be treating types as value types).
 *
 *		As we've designed them, types are essentially immutable,
 *		since we haven't included any mutators.  In practice, we'll
 *		be creating new types rather than changing existing types.
 */

# ifndef TYPE_H
# define TYPE_H
# include <string>
# include <vector>
# include <ostream>

typedef std::vector<class Type> Parameters;

class Type {
protected:
    typedef std::string string;
    enum { ARRAY, CALLBACK, ERROR, FUNCTION, SCALAR };

    int _kind;
    string _specifier;
    unsigned _indirection;

    union {
	unsigned _length;
	Parameters *_parameters;
    };

    Type(int kind, const string &specifier, unsigned indirection);

public:
    Type();

    bool operator ==(const Type &rhs) const;
    bool operator !=(const Type &rhs) const;

    bool isArray() const;
    bool isScalar() const;
    bool isFunction() const;
    bool isCallback() const;
    bool isError() const;

    const string &specifier() const;
    unsigned indirection() const;
    unsigned length() const;
    Parameters *parameters() const;

    bool isStruct() const;
    bool isValue() const;
    bool isInteger() const;
    bool isPointer() const;
    bool isCompatibleWith(const Type &that) const;

    Type promote() const;
    Type deref() const;

    unsigned size() const;
    unsigned alignment() const;
};

struct Scalar : public Type {
    Scalar(const string &specifier, unsigned indirection = 0);
};

struct Array : public Type {
    Array(const string &specifier, unsigned indirection, unsigned length);
};

struct Function : public Type {
    Function(const string &specifier, unsigned indirection,
	    Parameters *params = nullptr);
};

struct Callback : public Type {
    Callback(const string &specifier, unsigned indirection = 0);
};

std::ostream &operator <<(std::ostream &ostr, const Type &type);

# endif /* TYPE_H */
