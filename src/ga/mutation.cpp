#include "mutation.h"

#include <random>

using namespace ga;

Chromosome<GeneBin> ga::bitwiseMutation(std::mt19937& rng, const Chromosome<GeneBin>& chromosome, std::size_t binSize, double p_m){
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

Chromosome<GeneIntPerm> ga::swapMutation(std::mt19937& rng, const Chromosome<GeneIntPerm>& chromosome, double p_m){
    std::uniform_real_distribution<double> pDist(0.0, 1.0);
    Chromosome<GeneIntPerm> result = chromosome;

    if (pDist(rng) > p_m) return result;

    std::uniform_int_distribution<std::size_t> cDist(0, chromosome.size() - 1);
    std::size_t p1 {cDist(rng)}, p2;
    do{ p2 = cDist(rng); }while (p1 == p2);

    GeneIntPerm tmp = result.at(p1);
    result[p1] = result.at(p2);
    result[p2] = tmp;

    return result;
}