#include "functions.h"
#include <math.h>

double func1(double x){
    return cos(20.0 * x) - fabs(x) * .5 + x * x * x * 0.25;
}


// max f(st, lx) = 30 * st + 40 * lx            -> standard radio (st) profit: 30; luxurious radio (lx) profit: 40. Objective: max profit
// s.t. (1) st >= 0                                 -> amount of radios must not be negative
//      (2) lx >= 0                                 -> amount of radios must not be negative
//      (3) st * 1 <= 24 => st <= 24                -> a standard radio requires 1 worker/day. We can only allocate 24 workers in the assembly line
//      (4) lx * 2 <= 32 => lx <= 16                -> a luxurious radio requires 2 worker/day. We can only allocate 16 workers in the assembly line
//      (5) st * 1 + lx * 2 <= 40                   -> the factory only has 40 workers
double radioFactory(unsigned int st, unsigned int lx){
    return 30 * st + 40 * lx;
}

double radioFactoryR5(unsigned int st, unsigned int lx){
    return st + 2 * lx > 40 ? 40.0 - (double)(st + 2 * lx) : 0.0;
}