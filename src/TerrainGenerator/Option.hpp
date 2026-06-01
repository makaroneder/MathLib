#ifndef Option_H
#define Option_H
#include "Constraint.hpp"

struct Option : Constraint {
    uint32_t color;

    Option(void);
    Option(uint32_t color, const Block& upper, const Block& lower, const Block& left, const Block& right);
};

#endif