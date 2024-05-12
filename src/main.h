#ifndef MAIN_H
#define MAIN_H

enum RepresentationEnum {
    BINARY,
    INTEGER,
    INTEGER_PERM,
    REAL
};

enum SelectionEnum {
    ROULETTE,
    TOURNAMENT,
};

enum CrossoverEnum {
    ONE_POINT,
    UNIFORM,
    PMX,
    CX
};

enum MutationEnum {
    BITWISE,
    SWAP
};

enum ObjectiveEnum {
    FUNC1,
    SAT,
    RADIO,
    NQUEENS
};

#endif