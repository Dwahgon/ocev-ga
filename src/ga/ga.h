#ifndef GA_H
#define GA_H

#include "types.h"
#include "chromosomegen.h"
#include "populationgen.h"

#include <vector>
#include <functional>

namespace ga {
    template <class T>
    class GeneticAlgorithm{
        const unsigned long seed;
        const std::size_t populationSize;
        const PopulationGenerator<T> populationGenerator;
        const FitnessFunction<T> fitnessFunction;
        const SelectionFunction selectionFunction;
        const CrossoverFunction<T> crossoverFunction;
        const MutationFunction<T> mutationFunction;

        const double crossoverRate;
        const double mutationRate;

        Population<T> population;
        Scores populationScore;
        Chromosome<T> solution;
        Score solutionScore;
        int currentGeneration;
        std::mt19937 rng;
        std::vector<GenerationScoreInfo> convergenceTable;

        void setPopulation(const Population<T> population);

        public:
            GeneticAlgorithm(const unsigned long seed, std::size_t populationSize, const PopulationGenerator<T> populationGenerator, const FitnessFunction<T> fitnessFunction, const SelectionFunction selectionFunction, const CrossoverFunction<T> crossoverFunction, const MutationFunction<T> mutationFunction, double crossoverRate, double mutationRate);

            void initPopulation();
            Population<T> getPopulation() const;

            void calculatePopulationScore();
            Scores getPopulationScore() const;

            int getCurrentGeneration() const;

            GenerationScoreInfo calculateGenerationScoreInfo() const;

            std::size_t getCurrentBestIndividualIndex() const;
            std::size_t getCurrentWorstIndividualIndex() const;

            GeneticAlgorithmSolution<T> getSolution() const;
            std::vector<GenerationScoreInfo> getConvergenceTable() const;

            void step();

            void reset();
    };
}

#endif