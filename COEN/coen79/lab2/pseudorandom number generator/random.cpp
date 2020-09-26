#include <stdio.h>
#include <stdlib.h>
#include "random.h"

using namespace std;

namespace coen79_lab2 
{
    rand_gen::rand_gen(int seed, int multi, int incre, int mod)
    {
        //set all private variables to parameters
        this->seed = seed;
        this->multi= multi;
        this->incre = incre;
        this->mod = mod;
    }
    void rand_gen::set_seed(int new_seed)
    {
        //sets private variable seed to new seed
        this->seed = new_seed;
    }
    int rand_gen::next()
    {
        int temp;
        temp = (this->multi * this->seed + this->incre) % this->mod;
        this->seed = temp;
        return temp;
    }
    void rand_gen::print_info() const
    {
    }
}