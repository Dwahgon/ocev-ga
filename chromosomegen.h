#ifndef CHROMOSOMEGEN_H
#define CHROMOSOMEGEN_H

#include <vector>
#include <random>

template <class T>
using Chromosome = std::vector<T>;
using GeneBin = unsigned int;
using GeneInt = int;
using GeneIntPerm = unsigned int;
using GeneReal = double;

class ChromosomeGenerator
{
    const unsigned long seed;
    std::mt19937 rng;
    const unsigned long chromossomeSize;

public:
    ChromosomeGenerator(const unsigned long chromossomeSize);
    ChromosomeGenerator(const unsigned long chromossomeSize, const unsigned long seed);
    unsigned int getSeed() const;
    Chromosome<GeneBin> generateChromosomeBin();
    Chromosome<GeneBin> generateChromosomeBin(const std::size_t size);
    Chromosome<GeneInt> generateChromosomeInt(const int rangeStart, const int rangeEnd);
    Chromosome<GeneInt> generateChromosomeInt(const std::size_t size, const int rangeStart, const int rangeEnd);
    Chromosome<GeneIntPerm> generateChromosomeIntPerm();
    Chromosome<GeneIntPerm> generateChromosomeIntPerm(const std::size_t size);
    Chromosome<GeneReal> generateChromosomeReal(const double rangeStart, const double rangeEnd);
    Chromosome<GeneReal> generateChromosomeReal(const std::size_t size, const double rangeStart, const double rangeEnd);
};

#endif