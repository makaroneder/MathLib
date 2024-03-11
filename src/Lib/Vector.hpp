#ifndef Vector_H
#define Vector_H
#include "Matrix.hpp"

/// @brief Representation of axis position in matrix
enum class VectorAxis {
    X = 0,
    Y = 1,
    Z = 2,
};

#define GetVectorAxis(vector, axis) (vector).At((size_t)(axis), 0)

/// @brief Typedef for 2D vector
/// @tparam T Type of number
template <typename T>
using Vector2 = Matrix<T, 2, 1>;
/// @brief Typedef for 3D vector
/// @tparam T Type of number
template <typename T>
using Vector3 = Matrix<T, 3, 1>;

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
/// @brief Creates new 3D vector
/// @tparam T Type of number
/// @param x X coordinate
/// @param y Y coordinate
/// @param z Z coordinate
/// @return New 3D vector
template <typename T>
Vector3<T> CreateVector3(T x, T y, T z) {
    Vector3<T> ret;
    GetVectorAxis(ret, VectorAxis::X) = x;
    GetVectorAxis(ret, VectorAxis::Y) = y;
    GetVectorAxis(ret, VectorAxis::Z) = z;
    return ret;
}
/// @brief Creates new 3D vector
/// @tparam T Type of number
/// @param v 2D vector
/// @param z Z coordinate
/// @return New 3D vector
template <typename T>
Vector3<T> CreateVector3(Vector2<T> v, T z) {
    return CreateVector3<T>(GetVectorAxis(v, VectorAxis::X), GetVectorAxis(v, VectorAxis::Y), z);
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
    T sin = 0;
    T cos = 0;
    sincosl(angle, &sin, &cos);
    const T x = GetVectorAxis(point, VectorAxis::X);
    const T y = GetVectorAxis(point, VectorAxis::Y);
    return CreateVector2<T>(x * cos - y * sin, x * sin + y * cos) + origin;
}
/// @brief Converts 3D vector to 2D vector
/// @tparam T Type of number
/// @param point 3D vector
/// @return 2D vector
template <typename T>
Vector2<T> ConvertVector3ToVector2(Vector3<T> point) {
    const T fov = 10;
    const T x = GetVectorAxis(point, VectorAxis::X);
    const T y = GetVectorAxis(point, VectorAxis::Y);
    const T z = GetVectorAxis(point, VectorAxis::Z);
    return CreateVector2((fov * x) / (fov + z), (fov * y) / (fov + z));
}

#endif