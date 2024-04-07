#ifndef SAT_H
#define SAT_H

#include <vector>

namespace sat
{
    class Clause
    {
        const std::vector<int> clause;

    public:
        Clause();
        Clause(const std::vector<int> _clause);
        bool solve(const std::vector<unsigned int> &values) const;
        std::size_t score(const std::vector<unsigned int> &values) const;
        const std::vector<int> getClause() const;
    };

    class Formula
    {
        std::vector<Clause> clauses;
        const std::size_t varCount;

    public:
        Formula();
        Formula(const std::vector<Clause> _clauses);
        bool solve(std::vector<unsigned int> values) const;
        std::size_t score(const std::vector<unsigned int> &values) const;
        std::size_t getVarCount() const;
    };
}

#endif