#ifndef CHROMOSSOME_PRINTER_H
#define CHROMOSSOME_PRINTER_H

#include "chromosomegen.h"
#include "populationgen.h"

std::string binToString(int num, unsigned int sizeFixed = 0);

template <typename T>
std::string chromosomeToString(Chromosome<T> chromosome, unsigned int is_bin = 0);

template <typename T>
std::string populationToString(Population<T> population, unsigned int is_bin = 0);

#endif