#include "ga.h"
#include <algorithm>

using namespace ga;

template<class T>
GeneticAlgorithm<T>::GeneticAlgorithm(
    const std::size_t populationSize,
    const PopulationGenerator<T> populationGenerator,
    const ObjectiveFunction<T> objectiveFunction
)
    : populationSize(populationSize),
    populationGenerator(populationGenerator),
    objectiveFunction(objectiveFunction)
    {}
template GeneticAlgorithm<GeneBin>::GeneticAlgorithm(const std::size_t, const PopulationGenerator<GeneBin>, const ObjectiveFunction<GeneBin>);
template GeneticAlgorithm<GeneInt>::GeneticAlgorithm(const std::size_t, const PopulationGenerator<GeneInt>, const ObjectiveFunction<GeneInt>);
template GeneticAlgorithm<GeneReal>::GeneticAlgorithm(const std::size_t, const PopulationGenerator<GeneReal>, const ObjectiveFunction<GeneReal>);


template<class T>
void GeneticAlgorithm<T>::initPopulation(){
    this->population = this->populationGenerator(this->populationSize);
    this->calculatePopulationScore();
}
template void GeneticAlgorithm<GeneBin>::initPopulation();
template void GeneticAlgorithm<GeneInt>::initPopulation();
template void GeneticAlgorithm<GeneReal>::initPopulation();

template<class T>
Population<T> GeneticAlgorithm<T>::getPopulation() const {
    return this->population;
};
template Population<GeneBin> GeneticAlgorithm<GeneBin>::getPopulation() const;
template Population<GeneInt> GeneticAlgorithm<GeneInt>::getPopulation() const;
template Population<GeneReal> GeneticAlgorithm<GeneReal>::getPopulation() const;

template<class T>
void GeneticAlgorithm<T>::calculatePopulationScore(){
    this->populationScore.clear();
    for (const auto& chromossome : this->population){
        populationScore.push_back(this->objectiveFunction(chromossome));
    }
    this->bestIndividualIndex = std::distance(this->populationScore.begin(), std::max_element(this->populationScore.begin(), this->populationScore.end()));
}
template void GeneticAlgorithm<GeneBin>::calculatePopulationScore();
template void GeneticAlgorithm<GeneInt>::calculatePopulationScore();
template void GeneticAlgorithm<GeneReal>::calculatePopulationScore();

template<class T>
Scores GeneticAlgorithm<T>::getPopulationScore() const {
    return this->populationScore;
}
template Scores GeneticAlgorithm<GeneBin>::getPopulationScore() const;
template Scores GeneticAlgorithm<GeneInt>::getPopulationScore() const;
template Scores GeneticAlgorithm<GeneReal>::getPopulationScore() const;

template<class T>
std::size_t GeneticAlgorithm<T>::getBestIndividualIndex() const {
    return this->bestIndividualIndex;
};
template std::size_t GeneticAlgorithm<GeneBin>::getBestIndividualIndex() const;
template std::size_t GeneticAlgorithm<GeneInt>::getBestIndividualIndex() const;
template std::size_t GeneticAlgorithm<GeneReal>::getBestIndividualIndex() const;