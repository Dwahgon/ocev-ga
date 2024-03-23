#include "stringifiers.h"

#include <iostream>
#include <cstdint>
#include <iostream>
#include <sstream>

std::string binToString(unsigned int num, unsigned int sizeFixed)
{
    std::string res;
    int i = 0;
    while (sizeFixed || num)
    {
        res += num % 2 ? "1" : "0";
        if (!((++i) % 4)) res += " ";
        num = num >> 1;
        if (sizeFixed == 1)
            break;
        sizeFixed--;
    }
    return res;
}

template <typename T>
std::string chromosomeToString(ga::Chromosome<T> chromosome, unsigned int binSize)
{
    std::ostringstream stringStream;
    for (std::size_t i = 0; i < chromosome.size(); i++)
    {
        if (binSize)
            stringStream << binToString(chromosome.at(i), i != chromosome.size() - 1 ? 32 : binSize % 32);
        else
            stringStream << chromosome.at(i) << ' ';
    }
    return stringStream.str();
}
template std::string chromosomeToString(ga::Chromosome<ga::GeneBin>, unsigned int);
template std::string chromosomeToString(ga::Chromosome<ga::GeneInt>, unsigned int);
// template std::string chromosomeToString(ga::Chromosome<ga::GeneIntPerm>, unsigned int);
template std::string chromosomeToString(ga::Chromosome<ga::GeneReal>, unsigned int);

template <typename T>
std::string populationToString(ga::Population<T> population, unsigned int binSize)
{
    std::ostringstream stringStream;
    for (std::size_t i = 0; i < population.size(); i++)
        stringStream << i + 1 << ":\t" << chromosomeToString(population.at(i), binSize) << (i == population.size() - 1 ? "" : "\n");
    return stringStream.str();
}
template std::string populationToString(ga::Population<ga::GeneBin>, unsigned int);
template std::string populationToString(ga::Population<ga::GeneInt>, unsigned int);
// template std::string populationToString(ga::Population<ga::GeneIntPerm>, unsigned int);
template std::string populationToString(ga::Population<ga::GeneReal>, unsigned int);

std::string scoresToString(std::vector<int> scores){
    std::ostringstream stringStream;
    for (std::size_t i = 0; i < scores.size(); i++){
        std::cout << (i+1) << ":\t" << scores.at(i) << (i != scores.size() - 1 ? "\n" : " ");
    }
    return stringStream.str();
}

template <typename T>
std::string gaBestIndividualToString(ga::GeneticAlgorithm<T> &ga, std::size_t dim){
    std::size_t bestIndividualIndex {ga.getBestIndividualIndex()};
    ga::Population<T> population {ga.getPopulation()};
    ga::Scores scores {ga.getPopulationScore()};
    std::ostringstream stringStream;

    stringStream << bestIndividualIndex + 1 << ":\t"
        << chromosomeToString(population.at(bestIndividualIndex), dim)
        << "\nScore:\t" << scores.at(bestIndividualIndex);

    return stringStream.str();
}
template std::string gaBestIndividualToString(ga::GeneticAlgorithm<ga::GeneBin> &ga, std::size_t dim);
template std::string gaBestIndividualToString(ga::GeneticAlgorithm<ga::GeneInt> &ga, std::size_t dim);
// template std::string populationToString(ga::Population<ga::GeneIntPerm>, unsigned int);
template std::string gaBestIndividualToString(ga::GeneticAlgorithm<ga::GeneReal> &ga, std::size_t dim);