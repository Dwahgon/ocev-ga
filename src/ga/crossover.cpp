#include "crossover.h"

#include <random>

using namespace ga;


std::vector<Chromosome<GeneBin>> ga::binOnePointCrossover(std::mt19937& rng, const Chromosome<GeneBin>& father, const Chromosome<GeneBin>& mother, std::size_t binSize){
    std::uniform_int_distribution<std::size_t> dist(1, binSize - 2);

    std::size_t generatedNum = dist(rng);
    std::size_t index = generatedNum / 32, offset = generatedNum % 32;

    Chromosome<GeneBin> child1, child2;

    for (std::size_t i = 0; i <= father.size(); i++){
        if (i < index){
            child1.push_back(father.at(i));
            child2.push_back(mother.at(i));
        }else if (i > index){
            child1.push_back(mother.at(i));
            child2.push_back(father.at(i));
        }else{
            unsigned int fatherNum = father.at(i),
                        motherNum = mother.at(i);

            unsigned int fatherCutL = (fatherNum >> offset) << offset,
                        motherCutL = (motherNum >> offset) << offset,
                        fatherCutR = (fatherNum << (32 - offset)) >> (32 - offset),
                        motherCutR = (motherNum << (32 - offset)) >> (32 - offset);
            child1.push_back(motherCutL | fatherCutR);
            child2.push_back(fatherCutL | motherCutR);
        }
    }
    return {child1, child2};
}

std::vector<Chromosome<GeneBin>> ga::binUniformCrossover(std::mt19937& rng, const Chromosome<GeneBin>& father, const Chromosome<GeneBin>& mother){
    std::uniform_int_distribution<GeneBin> dist(0, UINT32_MAX);

    Chromosome<GeneBin> child1, child2;
    GeneBin mask;

    for (std::size_t i = 0; i < father.size(); i++){
        mask = dist(rng);
        child1.push_back((father.at(i) & ~mask) | (mother.at(i) & mask));
        child2.push_back((mother.at(i) & ~mask) | (father.at(i) & mask));
    }
    return {child1, child2};
}
