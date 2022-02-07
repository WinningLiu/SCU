#include <cassert>
#include "Type.h"

using namespace std;

Type::Type(int kind, const string &specifier, unsigned indirection) 
    : _kind(kind), _specifier(specifier), _indirection(indirection)
{
    _length = 0;
    _parameters = nullptr;
}

Type::Type(int kind, const string &specifier, unsigned indirection, unsigned len) 
    : _kind(kind), _specifier(specifier), _indirection(indirection), _length(len)
{
    _parameters = nullptr;
}

Type::Type(int kind, const string &specifier, unsigned indirection, Parameters* parameters) 
    : _kind(kind), _specifier(specifier), _indirection(indirection), _parameters(parameters)
{
    _length = 0;
}

Type::Type()
    :_kind(ERROR), _specifier("error"), _indirection(0)
{
    _length = 0;
    _parameters = nullptr;
}

bool Type::operator !=(const Type &rhs) const 
{
    return !operator==(rhs);
}

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

bool Type::isStruct() const
{
    return _kind != ERROR && _specifier != "char" && _specifier != "int";
}

ostream &operator <<(ostream &ostr, const Type &type)
{
    ostr << type.specifier();

    if (type.indirection() > 0) {
        ostr << " ";
        for (unsigned i = 0; i < type.indirection(); i++) {
            ostr << "*";
        }
    }

    if (type.isArray())
        ostr << "[" << type.length() << "]";
    else if (type.isCallback())
        ostr << "(*)()";
    else if (type.isFunction())
        ostr << "()";

    return ostr;
}