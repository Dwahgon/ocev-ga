#include "ga.h"

#include <random>
#include <algorithm>

using namespace ga;

template<class T>
GeneticAlgorithm<T>::GeneticAlgorithm(
    const std::size_t populationSize,
    const PopulationGenerator<T> populationGenerator,
    const ObjectiveFunction<T> objectiveFunction,
    const SelectionFunction selectionFunction,
    const CrossoverFunction<T> crossoverFunction,
    const MutationFunction<T> mutationFunction,
    double crossoverRate,
    double mutationRate
)
    : populationSize(populationSize),
    populationGenerator(populationGenerator),
    objectiveFunction(objectiveFunction),
    selectionFunction(selectionFunction),
    crossoverFunction(crossoverFunction),
    mutationFunction(mutationFunction),
    crossoverRate(crossoverRate),
    mutationRate(mutationRate),
    currentGeneration(0)
    {}
template GeneticAlgorithm<GeneInt>::GeneticAlgorithm(const std::size_t, const PopulationGenerator<GeneInt>, const ObjectiveFunction<GeneInt>, const SelectionFunction, const CrossoverFunction<GeneInt>, const MutationFunction<GeneInt>, double, double);
template GeneticAlgorithm<GeneBin>::GeneticAlgorithm(const std::size_t, const PopulationGenerator<GeneBin>, const ObjectiveFunction<GeneBin>, const SelectionFunction, const CrossoverFunction<GeneBin>, const MutationFunction<GeneBin>, double, double);
template GeneticAlgorithm<GeneReal>::GeneticAlgorithm(const std::size_t, const PopulationGenerator<GeneReal>, const ObjectiveFunction<GeneReal>, const SelectionFunction, const CrossoverFunction<GeneReal>, const MutationFunction<GeneReal>, double, double);


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

    std::size_t bestIndividualIndex = this->getCurrentBestIndividualIndex();
    if (populationScore.at(bestIndividualIndex) > this->solutionScore){
        this->solutionScore = populationScore.at(bestIndividualIndex);
        this->solution = this->population.at(bestIndividualIndex);
    }
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
std::size_t GeneticAlgorithm<T>::getCurrentBestIndividualIndex() const {
    return std::distance(this->populationScore.begin(), std::max_element(this->populationScore.begin(), this->populationScore.end()));
}
template std::size_t GeneticAlgorithm<GeneBin>::getCurrentBestIndividualIndex() const;
template std::size_t GeneticAlgorithm<GeneInt>::getCurrentBestIndividualIndex() const;
template std::size_t GeneticAlgorithm<GeneReal>::getCurrentBestIndividualIndex() const;

template<class T>
std::size_t GeneticAlgorithm<T>::getCurrentWorstIndividualIndex() const {
    return std::distance(this->populationScore.begin(), std::min_element(this->populationScore.begin(), this->populationScore.end()));
}
template std::size_t GeneticAlgorithm<GeneBin>::getCurrentWorstIndividualIndex() const;
template std::size_t GeneticAlgorithm<GeneInt>::getCurrentWorstIndividualIndex() const;
template std::size_t GeneticAlgorithm<GeneReal>::getCurrentWorstIndividualIndex() const;

template<class T>
void GeneticAlgorithm<T>::reset() {
    this->population.clear();
    this->populationScore.clear();
    this->solution = Chromosome<T>();
    this->solutionScore = 0;
    this->currentGeneration = 0;
}
template void GeneticAlgorithm<GeneBin>::reset();
template void GeneticAlgorithm<GeneInt>::reset();
template void GeneticAlgorithm<GeneReal>::reset();

template<class T>
void GeneticAlgorithm<T>::step() {
    if (!this->population.size()) return;
    std::mt19937 rng {std::random_device()()};
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    // Select individuals
    std::vector<int> selectedIndividuals = this->selectionFunction(this->populationSize, this->populationScore);

    // Create new population
    Population<T> newPopulation;

    // Crossover
    for (std::size_t i = 0; i < selectedIndividuals.size(); i += 2){
        if (dist(rng) < this->crossoverRate){
            std::vector<Chromosome<T>> crossoveredChildren = this->crossoverFunction(
                this->population.at(selectedIndividuals.at(i)),
                this->population.at(selectedIndividuals.at(i + 1))
            );
            newPopulation.insert(newPopulation.end(), crossoveredChildren.begin(), crossoveredChildren.end());
        }else{
            newPopulation.push_back(this->population.at(selectedIndividuals.at(i)));
            newPopulation.push_back(this->population.at(selectedIndividuals.at(i + 1)));
        }
    }

    // Mutation
    for (std::size_t i = 0; i < newPopulation.size(); i++){
        if (dist(rng) < this->mutationRate)
            newPopulation[i] = this->mutationFunction(newPopulation.at(i));
    }

    this->population = newPopulation;
    this->calculatePopulationScore();

    this->currentGeneration++;
}
template void GeneticAlgorithm<GeneBin>::step();
template void GeneticAlgorithm<GeneInt>::step();
template void GeneticAlgorithm<GeneReal>::step();

template<class T>
GenerationScoreInfo ga::GeneticAlgorithm<T>::calculateGenerationScoreInfo() const{
    Score bestScore {this->populationScore.at(this->getCurrentBestIndividualIndex())};
    Score worstScore {this->populationScore.at(this->getCurrentWorstIndividualIndex())};
    double avg {0}, stdDeviation {0};

    // Calculate average
    for (auto& score : this->populationScore) avg += (double)score;
    avg /= this->populationScore.size();

    // Calculate std deviation
    for (auto& score : this->populationScore) stdDeviation += ((double)score - avg) * ((double)score - avg);
    stdDeviation = sqrt(stdDeviation / this->populationScore.size());

    return {bestScore, worstScore, avg, stdDeviation};
}
template GenerationScoreInfo ga::GeneticAlgorithm<GeneBin>::calculateGenerationScoreInfo() const;
template GenerationScoreInfo ga::GeneticAlgorithm<GeneInt>::calculateGenerationScoreInfo() const;
template GenerationScoreInfo ga::GeneticAlgorithm<GeneReal>::calculateGenerationScoreInfo() const;

template<class T>
int ga::GeneticAlgorithm<T>::getCurrentGeneration() const{
    return this->currentGeneration;
}
template int ga::GeneticAlgorithm<GeneBin>::getCurrentGeneration() const;
template int ga::GeneticAlgorithm<GeneInt>::getCurrentGeneration() const;
template int ga::GeneticAlgorithm<GeneReal>::getCurrentGeneration() const;

template<class T>
GeneticAlgorithmSolution<T> ga::GeneticAlgorithm<T>::getSolution() const{
    return {this->solution, this->solutionScore};
}
template GeneticAlgorithmSolution<GeneBin> ga::GeneticAlgorithm<GeneBin>::getSolution() const;
template GeneticAlgorithmSolution<GeneInt> ga::GeneticAlgorithm<GeneInt>::getSolution() const;
template GeneticAlgorithmSolution<GeneReal> ga::GeneticAlgorithm<GeneReal>::getSolution() const;