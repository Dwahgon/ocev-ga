#ifndef CROSSOVER_H
#define CROSSOVER_H

#include "types.h"

namespace ga{
    std::vector<Chromosome<GeneBin>> binOnePointCrossover(Chromosome<GeneBin> father, Chromosome<GeneBin> mother, std::size_t binSize);
};

#endif