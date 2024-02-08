#ifndef Typedefs_H
#define Typedefs_H
#include <cmath>

/// @brief Defaults type for numbers
typedef long double num_t;
/// @brief Scale
static constexpr num_t pointMultiplier = 100;
/// @brief Maximal error in float comparison
static constexpr num_t eps = 1 / pointMultiplier;

#endif