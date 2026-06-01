#include "Constraint.hpp"

Constraint::Constraint(void) : upper(), lower(), left(), right() {}
Constraint::Constraint(const Block& upper, const Block& lower, const Block& left, const Block& right) : upper(upper), lower(lower), left(left), right(right) {}