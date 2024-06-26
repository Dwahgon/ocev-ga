#include "fitness.h"

#include <math.h>
#include <numeric>
#include <algorithm>

using namespace ga;

template<class T>
ga::Fitness<T>::Fitness(
    ObjectiveFunction<T> objectiveFunction,
    const std::vector<Restriction<T>>& inequalityRestrictions,
    const std::vector<Restriction<T>>& equalityRestrictions,
    double inequalityPenaltyCoef,
    double equalityPenaltyCoef,
    double worstCaseOffset,
    bool maximize
) : objectiveFunction(objectiveFunction),
    inequalityRestrictions(inequalityRestrictions),
    equalityRestrictions(equalityRestrictions),
    inequalityPenaltyCoef(inequalityPenaltyCoef),
    equalityPenaltyCoef(equalityPenaltyCoef),
    worstCaseOffset(worstCaseOffset),
    maximize(maximize)
{}
template ga::Fitness<GeneBin>::Fitness(ObjectiveFunction<GeneBin> objectiveFunction, const std::vector<Restriction<GeneBin>>& inequalityRestrictions, const std::vector<Restriction<GeneBin>>& equalityRestrictions, double inequalityPenaltyCoef, double equalityPenaltyCoef, double worstCaseOffset, bool maximize);
template ga::Fitness<GeneInt>::Fitness(ObjectiveFunction<GeneInt> objectiveFunction, const std::vector<Restriction<GeneInt>>& inequalityRestrictions, const std::vector<Restriction<GeneInt>>& equalityRestrictions, double inequalityPenaltyCoef, double equalityPenaltyCoef, double worstCaseOffset, bool maximize);
template ga::Fitness<GeneReal>::Fitness(ObjectiveFunction<GeneReal> objectiveFunction, const std::vector<Restriction<GeneReal>>& inequalityRestrictions, const std::vector<Restriction<GeneReal>>& equalityRestrictions, double inequalityPenaltyCoef, double equalityPenaltyCoef, double worstCaseOffset, bool maximize);


template<class T>
Score ga::Fitness<T>::score(const Chromosome<T>& chromosome) const{
    double inequalityPenaltySum {0.0}, equalityPenaltySum {0.0};

    for(auto& inequality : this->inequalityRestrictions)
        inequalityPenaltySum += inequality(chromosome);
    for(auto& equality : this->equalityRestrictions)
        equalityPenaltySum += equality(chromosome);

    double objectiveFunctionScore {this->objectiveFunction(chromosome)};

    return (this->maximize ? (objectiveFunctionScore + this->worstCaseOffset > 0.0 ? objectiveFunctionScore + this->worstCaseOffset : 0.0)
        : objectiveFunctionScore < this->worstCaseOffset ? this->worstCaseOffset - objectiveFunctionScore : 0.0) +
        this->inequalityPenaltyCoef * inequalityPenaltySum +
        this->equalityPenaltyCoef * equalityPenaltySum;
}
template Score ga::Fitness<GeneBin>::score(const Chromosome<GeneBin>& chromosome) const;
template Score ga::Fitness<GeneInt>::score(const Chromosome<GeneInt>& chromosome) const;
template Score ga::Fitness<GeneReal>::score(const Chromosome<GeneReal>& chromosome) const;

template<class T>
ga::BinaryToNumericConversionFitness<T>::BinaryToNumericConversionFitness(
    Fitness<T> originalFitness,
    std::vector<Range<T>> chromosomeRanges
) : originalFitness(originalFitness),
    chromosomeRanges(chromosomeRanges)
{
    for(const Range<T>& range : this->chromosomeRanges)
        bitSizes.push_back(ga::BinaryToNumericConversionFitness<T>::binToNumericBitSize(range.Xmin, range.Xmax, range.precision));
}
template ga::BinaryToNumericConversionFitness<GeneReal>::BinaryToNumericConversionFitness(Fitness<GeneReal> originalFitness, std::vector<Range<GeneReal>> chromosomeRanges);
template ga::BinaryToNumericConversionFitness<GeneInt>::BinaryToNumericConversionFitness(Fitness<GeneInt> originalFitness, std::vector<Range<GeneInt>> chromosomeRanges);
template ga::BinaryToNumericConversionFitness<unsigned int>::BinaryToNumericConversionFitness(Fitness<unsigned int> originalFitness, std::vector<Range<unsigned int>> chromosomeRanges);

template<class T>
std::size_t ga::BinaryToNumericConversionFitness<T>::binToNumericBitSize(T Xmin, T Xmax, std::size_t precision) {
    return ceil(log2((Xmax - Xmin + 1) * pow(10.0, (double)precision)));
}
template std::size_t ga::BinaryToNumericConversionFitness<GeneInt>::binToNumericBitSize(GeneInt Xmin, GeneInt Xmax, std::size_t decimalPrecision);
template std::size_t ga::BinaryToNumericConversionFitness<GeneReal>::binToNumericBitSize(GeneReal Xmin, GeneReal Xmax, std::size_t decimalPrecision);
template std::size_t ga::BinaryToNumericConversionFitness<unsigned int>::binToNumericBitSize(unsigned int Xmin, unsigned int Xmax, std::size_t decimalPrecision);

unsigned int extractDecimalNumberFromBinChromosome(Chromosome<GeneBin> chromosome, std::size_t index, std::size_t bitAmount){
    std::size_t startBitmapI {index},
        endBitmapI {index + bitAmount - 1},
        indexStart {startBitmapI / 32},
        indexEnd {endBitmapI / 32};
    unsigned int  maskStart = 0xffffffff << (startBitmapI % 32),
        maskEnd = 0xffffffff >> (31 - endBitmapI % 32);

    if (indexStart != indexEnd){
        unsigned int intStart = chromosome.at(indexStart) & maskStart,
            intEnd = chromosome.at(indexEnd) & maskEnd;
        return (intEnd << (endBitmapI % 32)) + (intStart >> (startBitmapI % 32));
    }
    return (chromosome.at(indexStart) & (maskStart & maskEnd)) >> (startBitmapI % 32);
}

template<class T>
T ga::BinaryToNumericConversionFitness<T>::binaryToRangeValue(unsigned int value, std::size_t bitSize, Range<T> range){
    double raw {(double)range.Xmin + ((double)(range.Xmax - range.Xmin) / (double)((1 << bitSize) - 1)) * (double)value};
    return (T)(range.precision == 0 ? round(raw) : raw);
}
template GeneBin ga::BinaryToNumericConversionFitness<GeneBin>::binaryToRangeValue(unsigned int value, std::size_t bitSize, Range<GeneBin> range);
template GeneReal ga::BinaryToNumericConversionFitness<GeneReal>::binaryToRangeValue(unsigned int value, std::size_t bitSize, Range<GeneReal> range);
template GeneInt ga::BinaryToNumericConversionFitness<GeneInt>::binaryToRangeValue(unsigned int value, std::size_t bitSize, Range<GeneInt> range);

template<class T>
Score ga::BinaryToNumericConversionFitness<T>::score(const Chromosome<GeneBin>& chromosome) const {
    Chromosome<T> convertedChromosome;

    Range<T> r;
    std::size_t bitSize, bitSizeAcc {0};
    for (std::size_t i {0}; i < this->chromosomeRanges.size(); i++){
        r = this->chromosomeRanges.at(i);
        bitSize = this->bitSizes.at(i);
        unsigned int dec {extractDecimalNumberFromBinChromosome(chromosome, bitSizeAcc, bitSize)};
        convertedChromosome.push_back(this->binaryToRangeValue(dec, bitSize, r));
        bitSizeAcc += bitSize;
    }

    return this->originalFitness.score(convertedChromosome);
}
template Score ga::BinaryToNumericConversionFitness<GeneInt>::score(const Chromosome<GeneBin>& chromosome) const;
template Score ga::BinaryToNumericConversionFitness<GeneReal>::score(const Chromosome<GeneBin>& chromosome) const;
template Score ga::BinaryToNumericConversionFitness<unsigned int>::score(const Chromosome<GeneBin>& chromosome) const;