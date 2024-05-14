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
        const std::size_t generationGap;
        const PopulationGenerator<T> populationGenerator;
        const FitnessFunction<T> fitnessFunction;
        const SelectionFunction selectionFunction;
        const CrossoverFunction<T> crossoverFunction;
        const MutationFunction<T> mutationFunction;
        const LinearScalingFunction linearScalingFunction;
        const bool linearScalingEnabled;

        const double crossoverRate;
        const double mutationRate;
        const std::size_t elitism;
        const short threads;

        Population<T> population;
        Scores populationScore;
        Chromosome<T> solution;
        Score solutionScore;
        int currentGeneration;
        double linearScalingC;
        std::mt19937 rng;
        std::vector<GenerationScoreInfo> convergenceTable;

        void setPopulation(const Population<T> population);

        Scores calculateScoresScaled() const;

        public:
            GeneticAlgorithm(
                const unsigned long seed,
                std::size_t populationSize,
                std::size_t generationGap,
                const PopulationGenerator<T> populationGenerator,
                const FitnessFunction<T> fitnessFunction,
                const SelectionFunction selectionFunction,
                const CrossoverFunction<T> crossoverFunction,
                const MutationFunction<T> mutationFunction,
                const LinearScalingFunction linearScalingFunction,
                const bool linearScalingEnabled,
                double crossoverRate,
                double mutationRate,
                bool elitism,
                short threads
            );

            void initPopulation();
            Population<T> getPopulation() const;

            void calculatePopulationScore();
            Scores getPopulationScore() const;

            int getCurrentGeneration() const;

            GenerationScoreInfo calculateGenerationScoreInfo() const;

            std::size_t getCurrentBestIndividualIndex() const;
            std::size_t getCurrentWorstIndividualIndex() const;
            double getCurrentAverageFitness() const;

            GeneticAlgorithmSolution<T> getSolution() const;
            std::vector<GenerationScoreInfo> getConvergenceTable() const;

            void step();

            void reset();
    };
}

#endif