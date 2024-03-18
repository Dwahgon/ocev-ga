#include "utils/confparser.h"
#include "ga/populationgen.h"
#include "ga/stringifiers.h"

#include <iostream>

int main()
{
        // Load conf
        confmap confs = parseConf("conf.conf");
        std::size_t dim = std::stoul(confs.at("DIM")),
                    pop = std::stoul(confs.at("POP"));
        GeneInt istart = std::stoi(confs.at("INT_RANGE_START")),
                iend = std::stoi(confs.at("INT_RANGE_END"));
        GeneReal rstart = std::stod(confs.at("REAL_RANGE_START")),
                 rend = std::stod(confs.at("REAL_RANGE_END"));

        // Print conf
        std::cout << "POP: " << dim << '\n'
                  << "DIM: " << pop << std::endl;

        // Create generator
        ChromosomeGenerator cg = confs.count("SEED") && confs.at("SEED").length() ? ChromosomeGenerator(dim, std::stoul(confs.at("SEED"))) : ChromosomeGenerator(dim);
        std::cout << "SEED: " << cg.getSeed() << std::endl;

        // Generate populations
        Population<GeneBin> binPop = generateGeneBinPopulation(cg, pop);
        Population<GeneInt> intPop = generateGeneIntPopulation(cg, pop, istart, iend);
        Population<GeneIntPerm> intPermPop = generateGeneIntPermPopulation(cg, pop);
        Population<GeneReal> realPop = generateGeneRealPopulation(cg, pop, rstart, rend);

        // Print populations
        std::cout << "POPULATION BINARY:\n"
                  << populationToString(binPop, dim) << '\n'
                  << "POPULATION INTEGER:\n"
                  << populationToString(intPop) << '\n'
                  << "POPULATION INTEGER PERMUTATION:\n"
                  << populationToString(intPermPop) << '\n'
                  << "POPULATION REAL:\n"
                  << populationToString(realPop) << '\n'
                  << std::endl;
}