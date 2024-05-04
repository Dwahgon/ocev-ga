#include "crossover.h"

#include <random>
#include <unordered_map>
#include <unordered_set>

using namespace ga;


std::vector<Chromosome<GeneBin>> ga::binOnePointCrossover(std::mt19937& rng, const Chromosome<GeneBin>& father, const Chromosome<GeneBin>& mother, std::size_t binSize){
    std::uniform_int_distribution<std::size_t> dist(1, binSize - 2);

    std::size_t generatedNum = dist(rng);
    std::size_t index = generatedNum / 32, offset = generatedNum % 32;

    Chromosome<GeneBin> child1, child2;

    for (std::size_t i = 0; i < father.size(); i++){
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


std::vector<Chromosome<GeneIntPerm>> ga::pmxCrossover(std::mt19937& rng, const Chromosome<GeneIntPerm>& father, const Chromosome<GeneIntPerm>& mother) {
    std::size_t p1, p2;
    Chromosome<GeneIntPerm> c1, c2;
    std::uniform_int_distribution<std::size_t> pDist(0, father.size()-1);

    p1 = pDist(rng);
    do{
        p2 = pDist(rng);
    }while(p2 == p1 || ((p1 == 0 && p2 == father.size() - 1) || (p1 == father.size() - 1 && p2 == 0)));
    if(p2 > p1){
        std::size_t tmp;
        tmp = p2;
        p2 = p1;
        p1 = tmp;
    }

    std::unordered_map<GeneIntPerm, GeneIntPerm> f2m, m2f;
    GeneIntPerm fi, mi;
    for (std::size_t i {p1}; i <= p2; i++){
        f2m[father.at(i)] = mother.at(i);
        m2f[mother.at(i)] = father.at(i);
    }
    for (std::size_t i {0}; i < father.size(); i++){
        fi = father.at(i);
        mi = mother.at(i);
        if (p1 <= i && i <= p2){
            c1.push_back(mi);
            c2.push_back(fi);
        }else{
            c1.push_back(m2f.count(fi) ? m2f.at(fi) : fi);
            c2.push_back(f2m.count(mi) ? f2m.at(mi) : mi);
        }
    }

    return {c1, c2};
}

std::vector<Chromosome<GeneIntPerm>> ga::cxCrossover(std::mt19937& rng, const Chromosome<GeneIntPerm>& father, const Chromosome<GeneIntPerm>& mother) {
    Chromosome<GeneIntPerm> c1 = father, c2 = mother;

    // Create mapping for father value -> index
    std::unordered_map<GeneIntPerm, std::size_t> f_v2i;
    for (std::size_t i {0}; i < father.size(); i++)
        f_v2i[father.at(i)] = i;

    // Create cycles
    std::vector<std::unordered_set<std::size_t>> cycles;
    for(std::size_t i {0}; i < father.size(); i++){
        // Ignore the current index if it's already in a cycle
        bool inCycle = false;
        for(std::size_t j {0}; j < cycles.size(); j++){
            if(cycles.at(j).count(i)){
                inCycle = true;
                break;
            }
        }
        if(inCycle) continue;

        cycles.push_back(std::unordered_set<std::size_t>());
        auto currentCycle = --cycles.end();

        std::size_t ix {i};
        do{
            (*currentCycle).emplace(ix);
            ix = f_v2i.at(mother.at(ix));
        }while(ix != i);
    }

    // Crossover
    for(std::size_t i {1}; i < cycles.size(); i+=2) {
        for (const std::size_t &ai : cycles.at(i)){
            c1[ai] = mother.at(ai);
            c2[ai] = father.at(ai);
        }
    }

    return {c1, c2};
}