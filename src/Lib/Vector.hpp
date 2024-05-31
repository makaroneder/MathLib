#ifndef Vector_H
#define Vector_H
#include "Matrix.hpp"

/// @brief Representation of axis position in matrix
enum class VectorAxis {
    X = 0,
    Y,
    Z,
    AxisCount,
};

#define GetVectorAxis(vector, axis) (vector).At((size_t)(axis), 0)

/// @brief x = v_0
/// @tparam T Type of number
/// @param v Vector
/// @return X
template <typename T>
constexpr T& GetX(Matrix<T>& v) {
    return GetVectorAxis(v, VectorAxis::X);
}
/// @brief y = v_1
/// @tparam T Type of number
/// @param v Vector
/// @return Y
template <typename T>
constexpr T& GetY(Matrix<T>& v) {
    return GetVectorAxis(v, VectorAxis::Y);
}
/// @brief z = v_2
/// @tparam T Type of number
/// @param v Vector
/// @return Z
template <typename T>
constexpr T& GetZ(Matrix<T>& v) {
    return GetVectorAxis(v, VectorAxis::Z);
}
/// @brief x = v_0
/// @tparam T Type of number
/// @param v Vector
/// @return X
template <typename T>
constexpr T GetX(const Matrix<T>& v) {
    return GetVectorAxis(v, VectorAxis::X);
}
/// @brief y = v_1
/// @tparam T Type of number
/// @param v Vector
/// @return Y
template <typename T>
constexpr T GetY(const Matrix<T>& v) {
    return GetVectorAxis(v, VectorAxis::Y);
}
/// @brief z = v_2
/// @tparam T Type of number
/// @param v Vector
/// @return Z
template <typename T>
constexpr T GetZ(const Matrix<T>& v) {
    return GetVectorAxis(v, VectorAxis::Z);
}
/// @brief Creates new 3D vector
/// @tparam T Type of number
/// @param x X coordinate
/// @param y Y coordinate
/// @param z Z coordinate
/// @return New 3D vector
template <typename T>
Matrix<T> CreateVector(T x, T y, T z) {
    Array<T> arr = Array<T>((size_t)VectorAxis::AxisCount);
    arr.At(0) = x;
    arr.At(1) = y;
    arr.At(2) = z;
    Matrix<T> ret = Matrix<T>(arr.GetSize(), 1, arr);
    return ret;
}
/// @brief Converts 3D vector to 2D vector
/// @tparam T Type of number
/// @param point 3D vector
/// @param fov Distance on Z axis between camera and origin
/// @return 2D vector
template <typename T>
Matrix<T> ConvertVectorToVector2(Matrix<T> point, T fov = 10) {
    if (GetZ(point) <= (1 - fov)) return CreateVector<T>(NAN, NAN, NAN);
    return CreateVector<T>(GetX(point), GetY(point), 0) / (1 + GetZ(point) / fov);
}

#endif