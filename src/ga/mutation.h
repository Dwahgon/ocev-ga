#ifndef MUTATION_H
#define MUTATION_H

#include "types.h"

namespace ga{
    Chromosome<GeneBin> bitwiseMutation(const Chromosome<GeneBin> chromosome, std::size_t binSize, double p_m);
}

#endif