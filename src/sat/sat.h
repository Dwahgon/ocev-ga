#ifndef SAT_H
#define SAT_H

#include <vector>

namespace sat
{
    class Clause
    {
        const std::vector<int> clause;

    public:
        Clause(const std::vector<int> _clause);
        bool solve(const std::vector<unsigned int> &values) const;
        std::size_t score(const std::vector<unsigned int> &values) const;
    };

    class Formula
    {
        std::vector<Clause> clauses;

    public:
        Formula(const std::vector<Clause> _clauses);
        bool solve(std::vector<unsigned int> values) const;
        std::size_t score(const std::vector<unsigned int> &values) const;
    };
}

#endif