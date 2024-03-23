#ifndef DIMACS_PARSER_H
#define DIMACS_PARSER_H

#include "sat.h"

#include <string>
#include <istream>


sat::Formula parseDimacsFormula(std::istream &is);

#endif