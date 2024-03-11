#include "populationgen.h"

Population<GeneBin> generateGeneBinPopulation(ChromosomeGenerator &cg, std::size_t pop_size)
{
    Population<GeneBin> population;
    for (std::size_t i = 1; i <= pop_size; i++)
        population.push_back(cg.generateChromosomeBin());
    return population;
}
Population<GeneInt> generateGeneIntPopulation(ChromosomeGenerator &cg, std::size_t pop_size, GeneInt range_start, GeneInt range_end)
{
    Population<GeneInt> population;
    for (std::size_t i = 1; i <= pop_size; i++)
        population.push_back(cg.generateChromosomeInt(range_start, range_end));
    return population;
}
Population<GeneIntPerm> generateGeneIntPermPopulation(ChromosomeGenerator &cg, std::size_t pop_size)
{
    Population<GeneIntPerm> population;
    for (std::size_t i = 1; i <= pop_size; i++)
        population.push_back(cg.generateChromosomeIntPerm());
    return population;
}
Population<GeneReal> generateGeneRealPopulation(ChromosomeGenerator &cg, std::size_t pop_size, GeneReal range_start, GeneReal range_end)
{
    Population<GeneReal> population;
    for (std::size_t i = 1; i <= pop_size; i++)
        population.push_back(cg.generateChromosomeReal(range_start, range_end));
    return population;
}