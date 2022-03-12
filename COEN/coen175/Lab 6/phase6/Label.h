#ifndef LABEL_H
#define LABEL_H

#include <iostream>
#include <string>

class Label {
    static unsigned _counter;
    unsigned _number;

    public :
        Label();
        unsigned number() const;
};

std::ostream &operator << (std::ostream &ostr, const Label &label);

#endif