#include "selection.h"

#include <cctype>
#include <vector>
#include <algorithm>
#include <random>

std::size_t binSearchCDF(const std::vector<double>& l, double n){
    if (n >= 1.0) return l.size() - 1;
    if (n <= 0.0) return 0;
    std::size_t iStart {0}, iEnd {l.size() - 1}, curI {(iStart + iEnd) / 2 };

    while (!((curI == 0 ? 0.0 : l.at(curI - 1)) < n && n <= l.at(curI) ))
    {
        if (n < l.at(curI)){
            iEnd = curI;
        }else{
            iStart = curI + 1;
        }
        curI = (iStart + iEnd) / 2;
    }
    return curI;
}

std::vector<int> ga::rouletteWheelSelection(std::mt19937& rng, std::size_t amount, ga::Scores scores){
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    // Calculate total population score
    int totalScore = 0;
    for (auto& n : scores) totalScore += n;

    // Calculate CDF
    std::vector<double> cdfScores;
    for (std::size_t i {0}; i < scores.size(); i++) cdfScores.push_back((!cdfScores.size() ? 0 : cdfScores.at(i-1)) + (double)scores.at(i) / (double)totalScore);

    std::vector<int> selectedIndividuals;
    while (amount--) selectedIndividuals.push_back(binSearchCDF(cdfScores, dist(rng)));

    return selectedIndividuals;
}