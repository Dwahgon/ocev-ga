#ifndef SELECTION_H
#define SELECTION_H

#include "types.h"

#include <random>

namespace ga {
    std::vector<std::size_t> rouletteWheelSelection(std::mt19937& rng, std::size_t amount, const Scores& scores);
    std::vector<std::size_t> tournamentSelection(std::mt19937& rng, std::size_t amount, const ga::Scores& scores, std::size_t k, std::size_t kp);
}

#endif