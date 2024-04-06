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

    using Score = double;
    using Scores = std::vector<double>;

    template <class T>
    using PopulationGenerator = std::function<Population<T>(std::size_t)>;
    template <class T>
    using FitnessFunction = std::function<Score(const Chromosome<T>&)>;
    using SelectionFunction = std::function<std::vector<std::size_t>(std::mt19937&, std::size_t, const Scores&)>;
    template <class T>
    using CrossoverFunction = std::function<std::vector<Chromosome<T>>(std::mt19937&, const Chromosome<T>&, const Chromosome<T>&)>;
    template <class T>
    using MutationFunction = std::function<Chromosome<T>(std::mt19937&, const Chromosome<T>&, double p_m)>;

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