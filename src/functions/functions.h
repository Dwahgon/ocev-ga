#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <vector>

double func1(double x);
double radioFactory(unsigned int st, unsigned int lx);
double radioFactoryR5(unsigned int st, unsigned int lx);
std::size_t nQueens(const std::vector<std::size_t> &queens, bool verbose = false);
double nQueensScored(const std::vector<std::size_t> &queens);
double nQueensScoredMax(std::size_t size);

#endif
