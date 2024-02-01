#ifndef Vector2_H
#define Vector2_H
#include "Matrix.hpp"

/// @brief Representation of axis position in matrix
enum class VectorAxis {
    X = 0,
    Y = 1,
};

#define GetVectorAxis(vector, axis) (vector).At((size_t)(axis), 0)

/// @brief Typedef for 2D vector
/// @tparam T Type of number
template <typename T>
using Vector2 = Matrix<T, 2, 1>;
/// @brief Creates new 2D vector
/// @tparam T Type of number
/// @param x X coordinate
/// @param y Y coordinate
/// @return New 2D vector
template <typename T>
Vector2<T> CreateVector2(T x, T y) {
    Vector2<T> ret;
    GetVectorAxis(ret, VectorAxis::X) = x;
    GetVectorAxis(ret, VectorAxis::Y) = y;
    return ret;
}

#endif