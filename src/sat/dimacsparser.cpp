#include "dimacsparser.h"

#include <sstream>
#include <string>
#include <istream>
#include <vector>

sat::Formula parseDimacsFormula(std::istream &is)
{
    std::string l;
    std::string word;
    std::vector<int> literals;
    std::vector<sat::Clause> clauses;
    while (std::getline(is, l))
    {
        if (l[0] == 'c' || l[0] == 'p' || l.empty())
            continue;
        else if (l[0] == '%')
            break;
        else
        {
            std::istringstream iss(l);
            while (iss >> word && word != "0")
            {
                literals.push_back(std::stoi(word));
            }
            clauses.push_back(sat::Clause(literals));
            literals.clear();
        }
    }
    return sat::Formula(clauses);
}