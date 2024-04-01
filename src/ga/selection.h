#ifndef SELECTION_H
#define SELECTION_H

#include "types.h"

#include <random>

namespace ga {
    std::vector<int> rouletteWheelSelection(std::mt19937& rng, std::size_t amount, const Scores scores);
}

#endif