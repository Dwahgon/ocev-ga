#include "sat.h"

#include <math.h>

sat::Clause::Clause() {}
sat::Clause::Clause(const std::vector<int> _clause) : clause(_clause) {}

std::size_t sat::Clause::score(const std::vector<unsigned int> &values) const
{
    std::size_t count = 0;
    for (const int var : this->clause)
    {
        int valuesIndex = (abs(var) - 1) / 32;
        int valueBitshift = (abs(var) - 1) % 32;
        int varBit = (values.at(valuesIndex) >> valueBitshift) % 2;
        if ((var > 0 && varBit) || (var < 0 && !varBit))
            count++;
    }
    return count;
}

bool sat::Clause::solve(const std::vector<unsigned int> &values) const
{
    for (const int var : this->clause)
    {
        int valuesIndex = (abs(var) - 1) / 32;
        int valueBitshift = (abs(var) - 1) % 32;
        int varBit = (values.at(valuesIndex) >> valueBitshift) % 2;
        if ((var > 0 && varBit) || (var < 0 && !varBit))
            return true;
    }
    return false;
}

const std::vector<int> sat::Clause::getClause() const{
    return this->clause;
}

std::size_t calculateVarCount(const std::vector<sat::Clause>& _clauses){
    std::size_t varCount {0};
    for(auto &clause : _clauses){
        for(auto &var : clause.getClause()){
            if (abs(var) > varCount)
                varCount = abs(var);
        }
    }
    return varCount;
}

sat::Formula::Formula() : varCount(0) {}
sat::Formula::Formula(const std::vector<Clause> _clauses) : clauses(_clauses), varCount(calculateVarCount(_clauses)) {}

std::size_t sat::Formula::score(const std::vector<unsigned int> &values) const
{
    std::size_t count = 0;
    for (const Clause &clause : this->clauses)
    {
        if (clause.solve(values))
            count++;
    }
    return count;
}

bool sat::Formula::solve(std::vector<unsigned int> values) const
{
    for (const Clause &clause : this->clauses)
    {
        if (!clause.solve(values))
            return false;
    }
    return true;
}

std::size_t sat::Formula::getVarCount() const{
    return this->varCount;
}

std::size_t sat::Formula::getClauseCount() const {
    return this->clauses.size();
}