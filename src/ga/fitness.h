#ifndef FITNESS_H
#define FITNESS_H

#include "types.h"

namespace ga{
    template <class T>
    using ObjectiveFunction = std::function<double(const std::vector<T>&)>;
    template <class T>
    using Restriction = std::function<double(const std::vector<T>&)>;

    template <class T>
    class Fitness {
        ObjectiveFunction<T> objectiveFunction;
        const std::vector<Restriction<T>> inequalityRestrictions;
        const std::vector<Restriction<T>> equalityRestrictions;

        const double inequalityPenaltyCoef, equalityPenaltyCoef, worstCaseOffset;
        const bool maximize;
    public:
        Fitness(ObjectiveFunction<T> objectiveFunction, const std::vector<Restriction<T>>& inequalityRestrictions, const std::vector<Restriction<T>>& equalityRestrictions, double inequalityPenaltyCoef, double equalityPenaltyCoef, double worstCaseOffset, bool maximize);

        virtual Score score(const Chromosome<T>& chromosome) const;
    };

    template<class T>
    struct Range{
        T Xmin, Xmax;
        std::size_t precision;
    };

    template <class T>
    class BinaryToNumericConversionFitness{
        Fitness<T> originalFitness;
        std::vector<Range<T>> chromosomeRanges;
        std::vector<std::size_t> bitSizes;
    public:
        BinaryToNumericConversionFitness(Fitness<T> originalFitness, std::vector<Range<T>> chromosomeRanges);

        Score score(const Chromosome<GeneBin>& chromosome) const;

        static std::size_t binToNumericBitSize(T Xmin, T Xman, std::size_t precision);
        static T binaryToRangeValue(unsigned int value, std::size_t bitSize, Range<T> range);
    };
}


#endif