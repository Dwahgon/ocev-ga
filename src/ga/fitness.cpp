#include "fitness.h"

#include <math.h>

using namespace ga;

std::size_t ga::calculateBinToRealBitSize(double Xmin, double Xmax, std::size_t decimalPrecision) {
    return ceil(log2((Xmax - Xmin) * pow(10.0, (double)decimalPrecision)));
}

std::size_t ga::calculateBinToIntBitSize(int Xmin, int Xmax) {
    return ceil(log2(Xmax - Xmin));
}

unsigned int extractBitsFromBinChromosome(Chromosome<GeneBin> chromosome, std::size_t index, std::size_t bitAmount){
    unsigned int startBitmapI = index == 0 ? 0 : index * bitAmount,
        endBitmapI = (index + 1) * bitAmount - 1;
    std::size_t indexStart = startBitmapI / 32,
        indexEnd = endBitmapI / 32;
    unsigned int  maskStart = 0xffffffff << (startBitmapI % 32),
        maskEnd = 0xffffffff >> (31 - endBitmapI % 32);

    if (indexStart != indexEnd){
        unsigned int intStart = chromosome.at(indexStart) & maskStart,
            intEnd = chromosome.at(indexEnd) & maskEnd;
        return (intEnd << (endBitmapI % 32)) + (intStart >> (startBitmapI % 32));
    }
    return (chromosome.at(indexStart) & (maskStart & maskEnd)) >> (startBitmapI % 32);
}

double binToRealFitness(std::function<double(std::vector<double>)> objectiveFunction, bool maximize, Chromosome<GeneBin> source, std::size_t sourceDim, double Xmin, double Xmax, int precision, double C){
    std::vector<double> objFuncInput;
    std::size_t L {calculateBinToRealBitSize(Xmin, Xmax, precision)},
        targetDim = (sourceDim - (sourceDim != 0)) / L + 1;
    for (std::size_t i {0}; i < targetDim; i++)
        objFuncInput.push_back(Xmin + (Xmax - Xmin) / ((1 << L) - 1) * extractBitsFromBinChromosome(source, i, L));
    double objFuncValue {objectiveFunction(objFuncInput)};
    return maximize ? (objFuncValue + C > 0.0 ? objFuncValue + C : 0.0)
        : (objFuncValue < C ? C - objFuncValue : 0.0); // M<
}

FitnessFunction<GeneBin> ga::createBinToRealFitness(std::function<double(std::vector<double>)> objectiveFunction, bool maximize, std::size_t sourceDim, double Xmin, double Xmax, int precision, double C){
    return std::bind(binToRealFitness, objectiveFunction, maximize, std::placeholders::_1, sourceDim, Xmin, Xmax, precision, C);
}