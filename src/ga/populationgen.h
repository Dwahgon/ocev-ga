#ifndef POPULATION_GEN_H
#define POPULATION_GEN_H

#include "types.h"
#include "chromosomegen.h"

namespace ga{
    Population<GeneBin> generateGeneBinPopulation(ChromosomeGenerator &cg, std::size_t pop_size);
    Population<GeneInt> generateGeneIntPopulation(ChromosomeGenerator &cg, std::size_t pop_size, GeneInt range_start, GeneInt range_end);
    Population<GeneIntPerm> generateGeneIntPermPopulation(ChromosomeGenerator &cg, std::size_t pop_size);
    Population<GeneReal> generateGeneRealPopulation(ChromosomeGenerator &cg, std::size_t pop_size, GeneReal range_start, GeneReal range_end);
}

#endif