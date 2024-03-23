#ifndef DIMACS_PARSER_H
#define DIMACS_PARSER_H

#include <string>
#include <istream>
#include "sat.h"


sat::Formula parseDimacsFormula(std::istream &is);

#endif