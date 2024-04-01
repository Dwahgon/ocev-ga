#ifndef SELECTION_FUNCTIONS_H
#define SELECTION_FUNCTIONS_H

#include "types.h"

namespace ga {
    std::vector<int> rouletteWheelSelection(std::size_t amount, const Scores scores);
}

#endif