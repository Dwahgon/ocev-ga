#include "selection.h"

std::vector<int> ga::rouletteWheelSelection(std::mt19937& rng, std::size_t amount, ga::Scores scores){
    // Calculate total population score
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    int totalScore {0};
    for (auto& n : scores) totalScore += n;

    // Calculate CDF
    Score scoreAcc;
    std::size_t lastScoreI {scores.size()};
    double rn, // Random number
        maxScoreAccRatio; // 1 / (Total score - score of lastScoreI)
    std::vector<int> selectedIndividuals;
    while (amount--){
        rn = dist(rng);
        scoreAcc = 0;
        maxScoreAccRatio = 1.0 / (double)(totalScore - (lastScoreI == scores.size() ? 0 : scores.at(lastScoreI)));
        for (std::size_t i {0}; i < scores.size(); i++){
            if (i == lastScoreI) continue;
            scoreAcc += scores.at(i);
            if (rn < (double)scoreAcc * maxScoreAccRatio) {
                selectedIndividuals.push_back(i);
                lastScoreI = i;
                break;
            }
        }
    }

    return selectedIndividuals;
}