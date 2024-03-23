#ifndef GA_H
#define GA_H

#include "chromosomegen.h"
#include "populationgen.h"

#include <vector>
#include <functional>


namespace ga {
    template <class T>
    using PopulationGenerator = std::function<Population<T>(std::size_t)>;
    template <class T>
    using ObjectiveFunction = std::function<int(Chromosome<T>)>;
    using Score = int;
    using Scores = std::vector<int>;

    template <class T>
    class GeneticAlgorithm{
        const std::size_t populationSize;
        const PopulationGenerator<T> populationGenerator;
        const ObjectiveFunction<T> objectiveFunction;

        Population<T> population;
        Scores populationScore;
        std::size_t bestIndividualIndex;

        public:
            GeneticAlgorithm(const std::size_t populationSize, const PopulationGenerator<T> populationGenerator, const ObjectiveFunction<T> objectiveFunction);

            void initPopulation();
            Population<T> getPopulation() const;

            void calculatePopulationScore();
            Scores getPopulationScore() const;

            std::size_t getBestIndividualIndex() const;
    };
}

#endif