#ifndef STATS_H
#define STATS_H
#include <iostream>

namespace coen79_lab2
{
    class statistician
    {
        public :
            statistician();
            void next(double var);
            int length() const;
            double sum() const;
            double mean() const;
            double minimum() const;
            double maximum() const;
            void reset();
            friend statistician operator +(const statistician &s1, const statistician &s2);
            friend statistician operator *(double scale, const statistician &s);
            friend bool operator ==(const statistician &s1, const statistician &s2);

        private :
            int len;
            double sum1;
            double min;
            double max;
    };
}



#endif