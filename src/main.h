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

enum LinearScalingEnum {
    LS_NONE,
    LINEAR,
    EASE_IN_OUT_CUBIC,
};

enum SteadyStateEnum {
    SS_NONE,
    UNIFORM_STEPS,
};

#endif