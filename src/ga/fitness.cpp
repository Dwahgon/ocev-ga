#include "fitness.h"

#include <math.h>
#include <numeric>

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
    T Xmin,
    T Xmax,
    std::size_t precision,
    std::size_t chromosomeDim
) : originalFitness(originalFitness),
    Xmin(Xmin),
    Xmax(Xmax),
    bitSize(BinaryToNumericConversionFitness<T>::binToNumericBitSize(Xmin, Xmax, precision)),
    chromosomeDim(chromosomeDim)
{}
template ga::BinaryToNumericConversionFitness<GeneReal>::BinaryToNumericConversionFitness(Fitness<GeneReal> originalFitness, GeneReal Xmin, GeneReal Xmax, std::size_t precision, std::size_t chromosomeDim);
template ga::BinaryToNumericConversionFitness<GeneInt>::BinaryToNumericConversionFitness(Fitness<GeneInt> originalFitness, GeneInt Xmin, GeneInt Xmax, std::size_t precision, std::size_t chromosomeDim);

template<class T>
std::size_t ga::BinaryToNumericConversionFitness<T>::binToNumericBitSize(T Xmin, T Xmax, std::size_t precision) {
    return ceil(log2((Xmax - Xmin) * pow(10.0, (double)precision)));
}
template std::size_t ga::BinaryToNumericConversionFitness<GeneInt>::binToNumericBitSize(GeneInt Xmin, GeneInt Xmax, std::size_t decimalPrecision);
template std::size_t ga::BinaryToNumericConversionFitness<GeneReal>::binToNumericBitSize(GeneReal Xmin, GeneReal Xmax, std::size_t decimalPrecision);

unsigned int extractDecimalNumberFromBinChromosome(Chromosome<GeneBin> chromosome, std::size_t index, std::size_t bitAmount){
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

template<class T>
Score ga::BinaryToNumericConversionFitness<T>::score(const Chromosome<GeneBin>& chromosome) const {
    Chromosome<T> convertedChromosome;
    std::size_t convertedChromosomeDim {(this->chromosomeDim - (this->chromosomeDim != 0)) / this->bitSize + 1};

    for(std::size_t i {0}; i < convertedChromosomeDim; i++)
        convertedChromosome.push_back(this->Xmin + (this->Xmax - this->Xmin) / ((1 << this->bitSize) - 1) * extractDecimalNumberFromBinChromosome(chromosome, i, this->bitSize));

    return this->originalFitness.score(convertedChromosome);
}
template Score ga::BinaryToNumericConversionFitness<GeneInt>::score(const Chromosome<GeneBin>& chromosome) const;
template Score ga::BinaryToNumericConversionFitness<GeneReal>::score(const Chromosome<GeneBin>& chromosome) const;