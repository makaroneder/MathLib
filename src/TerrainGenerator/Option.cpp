#include "Option.hpp"

Option::Option(void) : Constraint(), color(0) {}
Option::Option(uint32_t color, const Block& upper, const Block& lower, const Block& left, const Block& right) : Constraint(upper, lower, left, right), color(color) {}