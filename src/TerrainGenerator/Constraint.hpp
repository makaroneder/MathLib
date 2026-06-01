#ifndef Constraint_H
#define Constraint_H
#include "Block.hpp"

struct Constraint {
    Block upper;
    Block lower;
    Block left;
    Block right;

    Constraint(void);
    Constraint(const Block& upper, const Block& lower, const Block& left, const Block& right);
};

#endif