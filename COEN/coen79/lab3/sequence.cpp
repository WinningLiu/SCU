using namespace std;
#include <cassert>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include "sequence1.h"

namespace coen79_lab3
{
    sequence::sequence()
    {
        this->iterator = 0;
        this->length = 0;
    }
    void sequence::start()
    {
        iterator = 0;
    }
    void sequence::end()
    {
        iterator = length - 1;
    }
    void sequence::last()
    {
        iterator = CAPACITY - 1;
    }
    void sequence::advance()
    {
        assert(is_item());
        if (iterator < length)
            iterator++;
    }
    void sequence::retreat()
    {
        if (iterator > 0)
            iterator--;
    }
    void sequence::insert(const value_type& entry)
    {
        assert(size() < CAPACITY);
        if (length == 0)
        {
            data[length] = entry;
            iterator = 0;
            length++;
        }
        else
        {
            for (int i = length; i >= iterator + 1; i--)
            {
                data[i] = data[i-1];
            }
            data[iterator] = entry;
            length++;
        }
    }
    void sequence::attach(const value_type& entry)
    {
        assert(size() < CAPACITY);
        if (length == 0)
        {
            data[length] = entry;
            iterator = 0;
            length++;
        }
        else 
        {
            for (int i = length; i > iterator + 1; i--)
            {
                data[i] = data[i-1];
            }
            data[iterator+1] = entry;
            iterator++;
            length++;
        }

    }
    void sequence::remove_current()
    {
        assert(is_item());
        for (int i = iterator + 1; i < length; i++)
        {
            data[i - 1] = data[i];
        }
        length--;
    }
    void sequence::insert_front(const value_type& entry)
    {
        assert(size() < CAPACITY);
        iterator = 0;
        insert(entry);
        length++;
    }
    void sequence::attach_back(const value_type& entry)
    {
        assert(size() < CAPACITY);
        data[length] = entry;
        length++;
    }
    void sequence::remove_front()
    {
        assert(is_item());
        for (int i = 0; i < length - 1; i++)
        {
            data[i] = data[i+1];
        }
        length--;
    }
    void sequence::operator += (const sequence& rhs)
    {
        size_type temp = rhs.length;
        for (int i = 0; i < temp; i++)
        {
            data[length++] = rhs.data[i];
        }
    }
    sequence::size_type sequence::size() const
    {
        return length;
    }
    bool sequence::is_item() const
    {
        if (iterator < length)
            return true;
        return false;
    }
    sequence::value_type sequence::current() const
    {
        assert(is_item());
        return data[iterator];

    }
    sequence::value_type sequence::operator[](int index) const
    {  
        assert(index < length);
        return data[index];
    }
    double sequence::mean() const
    {
        int sum = 0;
        for (int i = 0; i < length; i++)
            sum += data[i];
        return (sum)/(length);
    }
    double sequence::standardDeviation() const
    {
        int temp = 0;
        for (int i = 0; i < length; i++)
        {
            temp += pow(data[i] - mean(), 2)/(length);
        }
        return sqrt(temp);
    }
    sequence operator +(const sequence& lsh, const sequence& rhs)
    {
        sequence temp(rhs);
        temp += lsh;
        return temp;
    }
    sequence::value_type summation(const sequence &s)
    {
        sequence::value_type temp = 0;
        for (int i = 0; i < s.size(); i++)
            temp += s[i];
        return temp;
    }
}

