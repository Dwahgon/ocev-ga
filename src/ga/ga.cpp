#include "ga.h"

#include <random>
#include <algorithm>
#include <thread>
#include <future>

using namespace ga;

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

template<class T>
GeneticAlgorithm<T>::GeneticAlgorithm(
    const unsigned long seed,
    const std::size_t populationSize,
    const PopulationGenerator<T> populationGenerator,
    const FitnessFunction<T> fitnessFunction,
    const SelectionFunction selectionFunction,
    const CrossoverFunction<T> crossoverFunction,
    const MutationFunction<T> mutationFunction,
    double crossoverRate,
    double mutationRate,
    bool elitism,
    short threads
)
    : seed(seed),
    populationSize(populationSize),
    populationGenerator(populationGenerator),
    fitnessFunction(fitnessFunction),
    selectionFunction(selectionFunction),
    crossoverFunction(crossoverFunction),
    mutationFunction(mutationFunction),
    crossoverRate(crossoverRate),
    mutationRate(mutationRate),
    elitism(elitism),
    threads(threads),
    currentGeneration(0),
    rng(std::mt19937(seed))
    {}
template GeneticAlgorithm<GeneInt>::GeneticAlgorithm(const unsigned long, const std::size_t, const PopulationGenerator<GeneInt>, const FitnessFunction<GeneInt>, const SelectionFunction, const CrossoverFunction<GeneInt>, const MutationFunction<GeneInt>, double, double, bool, short);
template GeneticAlgorithm<GeneIntPerm>::GeneticAlgorithm(const unsigned long, const std::size_t, const PopulationGenerator<GeneIntPerm>, const FitnessFunction<GeneIntPerm>, const SelectionFunction, const CrossoverFunction<GeneIntPerm>, const MutationFunction<GeneIntPerm>, double, double, bool, short);
template GeneticAlgorithm<GeneBin>::GeneticAlgorithm(const unsigned long, const std::size_t, const PopulationGenerator<GeneBin>, const FitnessFunction<GeneBin>, const SelectionFunction, const CrossoverFunction<GeneBin>, const MutationFunction<GeneBin>, double, double, bool, short);
template GeneticAlgorithm<GeneReal>::GeneticAlgorithm(const unsigned long, const std::size_t, const PopulationGenerator<GeneReal>, const FitnessFunction<GeneReal>, const SelectionFunction, const CrossoverFunction<GeneReal>, const MutationFunction<GeneReal>, double, double, bool, short);


template<class T>
void GeneticAlgorithm<T>::initPopulation(){
    this->setPopulation(this->populationGenerator(this->populationSize));
}
template void GeneticAlgorithm<GeneBin>::initPopulation();
template void GeneticAlgorithm<GeneInt>::initPopulation();
template void GeneticAlgorithm<GeneIntPerm>::initPopulation();
template void GeneticAlgorithm<GeneReal>::initPopulation();

template<class T>
Population<T> GeneticAlgorithm<T>::getPopulation() const {
    return this->population;
};
template Population<GeneBin> GeneticAlgorithm<GeneBin>::getPopulation() const;
template Population<GeneInt> GeneticAlgorithm<GeneInt>::getPopulation() const;
template Population<GeneIntPerm> GeneticAlgorithm<GeneIntPerm>::getPopulation() const;
template Population<GeneReal> GeneticAlgorithm<GeneReal>::getPopulation() const;

template<class T>
void GeneticAlgorithm<T>::calculatePopulationScore(){
    this->populationScore.clear();

    auto workloadFunction {[this](std::size_t ti, std::size_t workloadSize){
        Scores scores;
        std::size_t lastI = MIN(population.size(), (ti + 1) * workloadSize) - 1;
        for(std::size_t i {ti * workloadSize}; i <= lastI; i++){
            scores.push_back(fitnessFunction(population.at(i)));
        }
        return scores;
    }};

    if (this->threads > 0){
        std::vector<std::future<Scores>> workerThreads;
        std::size_t workloadSize {(std::size_t)std::ceil((double)this->populationSize / (double)this->threads)};

        for(short ti {0}; ti < this->threads; ti++){
            workerThreads.push_back(std::async([ti, workloadSize, workloadFunction](){ return workloadFunction(ti, workloadSize);}));
        }

        for (std::future<Scores>& workerThread : workerThreads){
            Scores workerScores {workerThread.get()};
            this->populationScore.insert(this->populationScore.end(), workerScores.begin(), workerScores.end());
        }
    }else{
        this->populationScore = workloadFunction(0, population.size());
    }

    std::size_t bestIndividualIndex = this->getCurrentBestIndividualIndex();
    if (populationScore.at(bestIndividualIndex) > this->solutionScore){
        this->solutionScore = populationScore.at(bestIndividualIndex);
        this->solution = this->population.at(bestIndividualIndex);
    }
}
template void GeneticAlgorithm<GeneBin>::calculatePopulationScore();
template void GeneticAlgorithm<GeneInt>::calculatePopulationScore();
template void GeneticAlgorithm<GeneIntPerm>::calculatePopulationScore();
template void GeneticAlgorithm<GeneReal>::calculatePopulationScore();

template<class T>
Scores GeneticAlgorithm<T>::getPopulationScore() const {
    return this->populationScore;
}
template Scores GeneticAlgorithm<GeneBin>::getPopulationScore() const;
template Scores GeneticAlgorithm<GeneInt>::getPopulationScore() const;
template Scores GeneticAlgorithm<GeneIntPerm>::getPopulationScore() const;
template Scores GeneticAlgorithm<GeneReal>::getPopulationScore() const;

template<class T>
std::size_t GeneticAlgorithm<T>::getCurrentBestIndividualIndex() const {
    return std::distance(this->populationScore.begin(), std::max_element(this->populationScore.begin(), this->populationScore.end()));
}
template std::size_t GeneticAlgorithm<GeneBin>::getCurrentBestIndividualIndex() const;
template std::size_t GeneticAlgorithm<GeneInt>::getCurrentBestIndividualIndex() const;
template std::size_t GeneticAlgorithm<GeneIntPerm>::getCurrentBestIndividualIndex() const;
template std::size_t GeneticAlgorithm<GeneReal>::getCurrentBestIndividualIndex() const;

template<class T>
std::size_t GeneticAlgorithm<T>::getCurrentWorstIndividualIndex() const {
    return std::distance(this->populationScore.begin(), std::min_element(this->populationScore.begin(), this->populationScore.end()));
}
template std::size_t GeneticAlgorithm<GeneBin>::getCurrentWorstIndividualIndex() const;
template std::size_t GeneticAlgorithm<GeneInt>::getCurrentWorstIndividualIndex() const;
template std::size_t GeneticAlgorithm<GeneIntPerm>::getCurrentWorstIndividualIndex() const;
template std::size_t GeneticAlgorithm<GeneReal>::getCurrentWorstIndividualIndex() const;

template<class T>
void GeneticAlgorithm<T>::reset() {
    this->population.clear();
    this->populationScore.clear();
    this->solution = Chromosome<T>();
    this->solutionScore = 0;
    this->currentGeneration = 0;
    this->rng.seed(this->seed);
    this->convergenceTable.clear();
}
template void GeneticAlgorithm<GeneBin>::reset();
template void GeneticAlgorithm<GeneInt>::reset();
template void GeneticAlgorithm<GeneIntPerm>::reset();
template void GeneticAlgorithm<GeneReal>::reset();

template<class T>
void GeneticAlgorithm<T>::setPopulation(const Population<T> population){
    this->population = population;
    this->calculatePopulationScore();
    this->convergenceTable.push_back(this->calculateGenerationScoreInfo());
}
template void GeneticAlgorithm<GeneBin>::setPopulation(const Population<GeneBin> population);
template void GeneticAlgorithm<GeneInt>::setPopulation(const Population<GeneInt> population);
template void GeneticAlgorithm<GeneIntPerm>::setPopulation(const Population<GeneIntPerm> population);
template void GeneticAlgorithm<GeneReal>::setPopulation(const Population<GeneReal> population);

template<class T>
void GeneticAlgorithm<T>::step() {
    if (!this->population.size()) return;
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    // Select individuals
    std::vector<std::size_t> selectedIndividuals = this->selectionFunction(this->rng, this->populationSize, this->populationScore);

    // Create new population
    Population<T> newPopulation;

    // Crossover
    for (std::size_t i = 0; i < selectedIndividuals.size(); i += 2){
        if (dist(this->rng) < this->crossoverRate){
            std::vector<Chromosome<T>> crossoveredChildren = this->crossoverFunction(
                this->rng,
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
        newPopulation[i] = this->mutationFunction(this->rng, newPopulation.at(i), this->mutationRate);
    }

    // Elitism
    if (this->elitism){
        std::size_t worstI = this->getCurrentWorstIndividualIndex();
        newPopulation[worstI] = this->solution;
    }


    this->setPopulation(newPopulation);

    this->currentGeneration++;
}
template void GeneticAlgorithm<GeneBin>::step();
template void GeneticAlgorithm<GeneInt>::step();
template void GeneticAlgorithm<GeneIntPerm>::step();
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
template GenerationScoreInfo ga::GeneticAlgorithm<GeneIntPerm>::calculateGenerationScoreInfo() const;
template GenerationScoreInfo ga::GeneticAlgorithm<GeneReal>::calculateGenerationScoreInfo() const;

template<class T>
int ga::GeneticAlgorithm<T>::getCurrentGeneration() const{
    return this->currentGeneration;
}
template int ga::GeneticAlgorithm<GeneBin>::getCurrentGeneration() const;
template int ga::GeneticAlgorithm<GeneInt>::getCurrentGeneration() const;
template int ga::GeneticAlgorithm<GeneIntPerm>::getCurrentGeneration() const;
template int ga::GeneticAlgorithm<GeneReal>::getCurrentGeneration() const;

template<class T>
GeneticAlgorithmSolution<T> ga::GeneticAlgorithm<T>::getSolution() const{
    return {this->solution, this->solutionScore};
}
template GeneticAlgorithmSolution<GeneBin> ga::GeneticAlgorithm<GeneBin>::getSolution() const;
template GeneticAlgorithmSolution<GeneInt> ga::GeneticAlgorithm<GeneInt>::getSolution() const;
template GeneticAlgorithmSolution<GeneIntPerm> ga::GeneticAlgorithm<GeneIntPerm>::getSolution() const;
template GeneticAlgorithmSolution<GeneReal> ga::GeneticAlgorithm<GeneReal>::getSolution() const;

template<class T>
std::vector<GenerationScoreInfo> GeneticAlgorithm<T>::getConvergenceTable() const{
    return this->convergenceTable;
}
template std::vector<GenerationScoreInfo> ga::GeneticAlgorithm<GeneBin>::getConvergenceTable() const;
template std::vector<GenerationScoreInfo> ga::GeneticAlgorithm<GeneInt>::getConvergenceTable() const;
template std::vector<GenerationScoreInfo> ga::GeneticAlgorithm<GeneIntPerm>::getConvergenceTable() const;
template std::vector<GenerationScoreInfo> ga::GeneticAlgorithm<GeneReal>::getConvergenceTable() const;