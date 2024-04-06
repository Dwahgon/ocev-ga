#ifndef CROSSOVER_H
#define CROSSOVER_H

#include "types.h"

#include <random>

namespace ga{
    std::vector<Chromosome<GeneBin>> binOnePointCrossover(std::mt19937& rng, const Chromosome<GeneBin>& father, const Chromosome<GeneBin>& mother, std::size_t binSize);
};

#endif