#include <iostream>
#include "confparser.h"
#include "populationgen.h"
#include "stringifiers.h"
#include "dimacsparser.h"
#include "ga.h"

int main()
{
    // Load conf
    confmap confs {parseConf("conf.conf")};
    std::size_t dim {std::stoul(confs.at("DIM"))},
        pop {std::stoul(confs.at("POP"))};
    [[maybe_unused]]
    ga::GeneInt istart {std::stoi(confs.at("INT_RANGE_START"))},
        iend {std::stoi(confs.at("INT_RANGE_END"))};
    [[maybe_unused]]
    ga::GeneReal rstart {std::stod(confs.at("REAL_RANGE_START"))},
        rend {std::stod(confs.at("REAL_RANGE_END"))};

    // Print conf
    std::cout << "POP: " << dim << '\n'
        << "DIM: " << pop << std::endl;

    // Create generator
    ga::ChromosomeGenerator cg {confs.count("SEED") && confs.at("SEED").length() ? ga::ChromosomeGenerator(dim, std::stoul(confs.at("SEED"))) : ga::ChromosomeGenerator(dim)};
    std::cout << "SEED: " << cg.getSeed() << std::endl;

    #if 0 // Assignment 1
        // Generate populations
        Population<ga::GeneBin> binPop = generateGeneBinPopulation(cg, pop);
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
    #endif

    #if 1 // Assignment 2
        sat::Formula formula {parseDimacsFormula(std::cin)};
        ga::GeneticAlgorithm<ga::GeneBin> geneticAlgorithm {
            pop,
            std::bind(ga::generateGeneBinPopulation, std::ref(cg), std::placeholders::_1),
            std::bind(&sat::Formula::score, &formula, std::placeholders::_1),
        };

        geneticAlgorithm.initPopulation();

        ga::Population<ga::GeneBin> gaPopulation {geneticAlgorithm.getPopulation()};
        ga::Scores scores {geneticAlgorithm.getPopulationScore()};

        std::cout << "Initial Population:\n" << populationToString(gaPopulation, dim)
                << "\nScores:\n" << scoresToString(scores)
                << "\nBest Individual:\n" << gaBestIndividualToString(geneticAlgorithm, dim)
                << std::endl;
    #endif
}