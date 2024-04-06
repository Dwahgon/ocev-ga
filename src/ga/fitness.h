#ifndef FITNESS_H
#define FITNESS_H

#include "types.h"

namespace ga{
    std::size_t calculateBinToRealBitSize(double xMin, double xMax, std::size_t decimalPrecision);
    std::size_t calculateBinToIntBitSize(int xMin, int xMax);
    FitnessFunction<GeneBin> createBinToRealFitness(std::function<double(const std::vector<double>&)> objectiveFunction, bool maximize, std::size_t sourceDim, double Xmax, double Xmin, int precision, double C);
    template<class T>
    FitnessFunction<T> objectiveIntToScore(std::function<int(const Chromosome<T>&)> objectiveFunction);
}


#endif