#include "stringifiers.h"

#include <iostream>
#include <cstdint>
#include <iostream>
#include <sstream>

std::string binToString(int num, unsigned int sizeFixed)
{
    std::string res;
    while (sizeFixed || num)
    {
        res += num % 2 ? "0" : "1";
        num = num >> 1;
        if (sizeFixed == 1)
            break;
        sizeFixed--;
    }
    return res;
}

template <typename T>
std::string chromosomeToString(Chromosome<T> chromosome, unsigned int binSize)
{
    std::ostringstream stringStream;
    for (const auto &i : chromosome)
    {
        if (binSize)
            stringStream << binToString(i, binSize);
        else
            stringStream << i << ' ';
    }
    return stringStream.str();
}
template std::string chromosomeToString(Chromosome<GeneBin>, unsigned int);
template std::string chromosomeToString(Chromosome<GeneInt>, unsigned int);
// template std::string chromosomeToString(Chromosome<GeneIntPerm>, unsigned int);
template std::string chromosomeToString(Chromosome<GeneReal>, unsigned int);

template <typename T>
std::string populationToString(Population<T> population, unsigned int binSize)
{
    std::ostringstream stringStream;
    for (std::size_t i = 0; i < population.size(); i++)
        stringStream << i + 1 << ":\t" << chromosomeToString(population.at(i), binSize) << (i == population.size() - 1 ? "" : "\n");
    return stringStream.str();
}
template std::string populationToString(Population<GeneBin>, unsigned int);
template std::string populationToString(Population<GeneInt>, unsigned int);
// template std::string populationToString(Population<GeneIntPerm>, unsigned int);
template std::string populationToString(Population<GeneReal>, unsigned int);