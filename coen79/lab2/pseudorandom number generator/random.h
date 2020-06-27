#ifndef RAND_NUM_GEN_H
#define RAND_NUM_GEN_H

#include <iostream>
#include <cassert>

using namespace std;

namespace coen79_lab2
{
    class rand_gen
    {
        public:
            rand_gen(int seed, int multi, int incre, int mod);
            void set_seed(int new_seed);
            int next();
            void print_info() const;

        private:
            int multi;
            int seed;
            int incre;
            int mod;
    };
}

#endif