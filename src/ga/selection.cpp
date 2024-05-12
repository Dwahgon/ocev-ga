#include "selection.h"

#include <algorithm>
#include <iostream>

std::vector<std::size_t> ga::rouletteWheelSelection(std::mt19937& rng, std::size_t amount, const ga::Scores& scores){
    // Calculate total population score
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    Score totalScore {0};
    for (auto& n : scores) totalScore += n;

    // Calculate CDF
    Score scoreAcc;
    std::size_t lastScoreI {scores.size()};
    double rn, // Random number
        maxScoreAccRatio; // 1 / (Total score - score of lastScoreI)
    std::vector<std::size_t> selectedIndividuals;
    while (amount--){
        rn = dist(rng);
        scoreAcc = 0;
        maxScoreAccRatio = 1.0 / (totalScore - (lastScoreI == scores.size() ? 0.0 : scores.at(lastScoreI)));
        for (std::size_t i {0}; i < scores.size(); i++){
            if (i == lastScoreI) continue;
            scoreAcc += scores.at(i);
            if (rn < (double)scoreAcc * maxScoreAccRatio) {
                selectedIndividuals.push_back(i);
                lastScoreI = lastScoreI < scores.size() ? i : scores.size();
                break;
            }
        }
    }

    return selectedIndividuals;
}

std::vector<std::size_t> ga::tournamentSelection(std::mt19937& rng, std::size_t amount, const ga::Scores& scores, std::size_t k, std::size_t kp){
    std::uniform_int_distribution<std::size_t> selectChromosomeDist(0, scores.size()-1);
    std::uniform_real_distribution<double> kpRandomNumber(0.0, 1.0);

    std::size_t rc, lastChromosome = scores.size();
    std::vector<std::size_t> selectedIndividuals;
    std::vector<std::size_t> tournament;
    while (amount--){
        // Select k chromosomes
        for(std::size_t i {0}; i < k; i++){
            do {
                rc = selectChromosomeDist(rng);
            }
            while(rc == lastChromosome || std::find(tournament.begin(), tournament.end(), rc) != tournament.end());
            tournament.push_back(rc);
        }
        std::sort(tournament.begin(), tournament.end(), [scores](std::size_t i, std::size_t j){return scores.at(i) < scores.at(j);});
        std::size_t chosenChromosome {kpRandomNumber(rng) <= kp ? tournament.back() : tournament.front()};
        lastChromosome = lastChromosome < scores.size() ? scores.size() : chosenChromosome;
        selectedIndividuals.push_back(chosenChromosome);
        tournament.clear();
    }

    return selectedIndividuals;
}