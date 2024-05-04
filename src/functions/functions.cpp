#include "functions.h"
#include <math.h>
#include <vector>

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
    return (30.0 * (double)st + 40.0 * (double)lx) / 1360.0;
}

double radioFactoryR5(unsigned int st, unsigned int lx){
    return st + 2 * lx > 40 ? ((double)(st + 2 * lx) - 40.0)/16.0 : 0.0;
}

std::size_t nQueens(const std::vector<std::size_t> &queens){
    std::size_t checked {0};
    for(std::size_t i {0}; i < queens.size() - 1; i++){
        std::size_t qi {queens.at(i)};
        for(std::size_t j {i+1}; j < queens.size(); j++){
            std::size_t qj {queens.at(j)};
            if(qi < qj ? qj - (j - i) == qi : qi - (j - i) == qj){
                checked++;
                break;
            }
        }
    }
    return checked;
}