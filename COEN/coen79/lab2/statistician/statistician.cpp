#include <iostream>
#include <cassert>
#include <stdio.h>
#include "statistician.h"

using namespace std;

namespace coen79_lab2
{
    statistician::statistician() 
    {
        this->reset();
    }
    void statistician::next(double var)
    {
        //if the new var is greater than the max, set the new max
        if (var > this->max)
            this->max = var;
        //if the new var is less than the min, set the new min
        if (var < this->min)
            this->min = var;
        //Add one to the length
        this->len++;
        //Add var to the sum
        this->sum1 += var; 
    }
    int statistician::length() const
    {
        return this->len;
    }
    double statistician::sum() const
    {
        return this->sum1;
    }
    double statistician::mean() const
    {
        //Length cannot be equal to zero
        assert(this->len > 0);
        return (this->sum1)/(this->len);
    }
    double statistician::minimum() const
    {
        assert(this->len > 0);
        return this->min;
    }
    double statistician::maximum() const
    {
        assert(this->len > 0);
        return this->max;
    }
    void statistician::reset()
    {
        this->len = 0;
        this->min = INT_MAX; //For ensurance, set min and max to int max and int min
        this->max = INT_MIN;
        this->sum1 = 0;
    }
    statistician operator +(const statistician &s1, const statistician &s2)
    {
    statistician temp(s1); //Set a temp class, since s1 is a const
    temp.len += s2.len;
    temp.sum1 += s2.sum1;
    temp.min = (temp.min < s2.min) ? temp.min : s2.min; //returns the min and max between the two classes
    temp.max = (temp.max > s2.max) ? temp.max : s2.max;
    return temp;
    }
    statistician operator *(double scale, const statistician &s)
    {
    statistician temp(s);
    temp.sum1 *= scale; //Multiplies sum, min and max by scale
    temp.min *= scale;
    temp.max *= scale;
    return temp;
    }
    bool operator ==(const statistician &s1, const statistician &s2)
    {
    if (s1.length() == 0 && s2.length() == 0)
        return true;
    else if (s1.length() == s2.length() && s1.minimum() == s2.minimum() && s1.maximum() == s2.maximum() && s1.sum() == s2.sum()) 
        return true;
    else 
        return false;
    }
}