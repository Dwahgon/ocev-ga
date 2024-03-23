#include "ga/ga.h"

#include "utils/confparser.h"
#include "utils/stringifiers.h"
#include "sat/dimacsparser.h"

#include <iostream>

#define ASSIGNMENT_NUM 2

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

    #if ASSIGNMENT_NUM == 1 // Assignment 1
        // Generate populations
        ga::Population<ga::GeneBin> binPop = generateGeneBinPopulation(cg, pop);
        ga::Population<ga::GeneInt> intPop = generateGeneIntPopulation(cg, pop, istart, iend);
        ga::Population<ga::GeneIntPerm> intPermPop = generateGeneIntPermPopulation(cg, pop);
        ga::Population<ga::GeneReal> realPop = generateGeneRealPopulation(cg, pop, rstart, rend);

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

    #if ASSIGNMENT_NUM == 2 // Assignment 2
        // Load SAT formula
        sat::Formula formula {parseDimacsFormula(std::cin)};

        // Configure GA
        ga::GeneticAlgorithm<ga::GeneBin> geneticAlgorithm {
            pop,
            std::bind(ga::generateGeneBinPopulation, std::ref(cg), std::placeholders::_1),
            std::bind(&sat::Formula::score, &formula, std::placeholders::_1),
        };

        // Create initial population
        geneticAlgorithm.initPopulation();

        // Get GA population data
        ga::Population<ga::GeneBin> gaPopulation {geneticAlgorithm.getPopulation()};
        ga::Scores scores {geneticAlgorithm.getPopulationScore()};

        // Print GA population data
        std::cout << "Initial Population:\n" << populationToString(gaPopulation, dim)
                << "\nScores:\n" << scoresToString(scores)
                << "\nBest Individual:\n" << gaBestIndividualToString(geneticAlgorithm, dim)
                << std::endl;
    #endif
}