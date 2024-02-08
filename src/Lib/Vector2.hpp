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
/// @brief (x', y') = (x cos a - y sin a, x sin a + y cos a)
/// @tparam T Type of number
/// @param point 2D vector to rotate
/// @param origin 2D vector to rotate around
/// @param angle Angle to rotate by
/// @return Rotated 2D vector
template <typename T>
Vector2<T> RotateVector2(Vector2<T> point, Vector2<T> origin, T angle) {
    point -= origin;
    const T sin = std::sin(angle);
    const T cos = std::cos(angle);
    const T x = GetVectorAxis(point, VectorAxis::X);
    const T y = GetVectorAxis(point, VectorAxis::Y);
    return CreateVector2<T>(x * cos - y * sin, x * sin + y * cos) + origin;
}

#endif