#ifndef MUTATION_H
#define MUTATION_H

#include "types.h"

namespace ga{
    Chromosome<GeneBin> bitwiseMutation(std::mt19937& rng, const Chromosome<GeneBin>& chromosome, std::size_t binSize, double p_m);
    Chromosome<GeneIntPerm> swapMutation(std::mt19937& rng, const Chromosome<GeneIntPerm>& chromosome, double p_m);
}

#endif