#include "mutation.h"

#include <random>

using namespace ga;

Chromosome<GeneBin> ga::bitwiseMutation(const Chromosome<GeneBin> chromosome, std::size_t binSize, double p_m){
    std::mt19937 rng {std::random_device()()};
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    Chromosome<GeneBin> result;
    GeneBin currentGene, mask;
    unsigned int lastIndex;

    for (std::size_t i = 0; i < chromosome.size(); i++){
        currentGene = chromosome.at(i);
        lastIndex = i == chromosome.size() - 1 ? (binSize - 1) % 32 : 32;
        mask = 0;
        for (unsigned int j = 0; j < lastIndex; j++){
            mask = (mask << 1) + (dist(rng) < p_m);
        }
        result.push_back(currentGene ^ mask);
    }

    return result;
}