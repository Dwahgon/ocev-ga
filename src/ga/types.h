#ifndef TYPES_H
#define TYPES_H

#include <functional>
#include <vector>
#include <random>

namespace ga
{
    template <class T>
    using Chromosome = std::vector<T>;
    using GeneBin = unsigned int;
    using GeneInt = int;
    using GeneIntPerm = unsigned int;
    using GeneReal = double;

    template <typename T>
    using Population = std::vector<Chromosome<T>>;

    using Score = int;
    using Scores = std::vector<int>;

    template <class T>
    using PopulationGenerator = std::function<Population<T>(std::size_t)>;
    template <class T>
    using ObjectiveFunction = std::function<int(Chromosome<T>)>;
    using SelectionFunction = std::function<std::vector<int>(std::mt19937&, std::size_t, const Scores)>;
    template <class T>
    using CrossoverFunction = std::function<std::vector<Chromosome<T>>(std::mt19937&, Chromosome<T>, Chromosome<T>)>;
    template <class T>
    using MutationFunction = std::function<Chromosome<T>(std::mt19937&, Chromosome<T>)>;

    struct GenerationScoreInfo {
        Score best, worst;
        double avg, stdDeviation;
    };

    template<class T>
    struct GeneticAlgorithmSolution {
        Chromosome<T> chromosome;
        Score score;
    };
}

#endif