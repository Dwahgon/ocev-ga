#ifndef CHROMOSSOME_PRINTER_H
#define CHROMOSSOME_PRINTER_H

#include "../ga/ga.h"

std::string binToString(unsigned int num, unsigned int sizeFixed = 0);

template <typename T>
std::string chromosomeToString(ga::Chromosome<T> chromosome, unsigned int binSize = 0);

template <typename T>
std::string populationToString(ga::Population<T> population, unsigned int binSize = 0);

std::string scoresToString(ga::Scores score);

template <typename T>
std::string gaBestIndividualToString(ga::GeneticAlgorithm<T> &ga, std::size_t dim);

template <typename T>
std::string gaGenerationScoreInfoToString(ga::GeneticAlgorithm<T> &ga);

template <typename T>
std::string gaSolutionToString(ga::GeneticAlgorithm<T> &ga, std::size_t dim);

#endif