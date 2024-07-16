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
#define GetX(vector) GetVectorAxis(vector, VectorAxis::X)
#define GetY(vector) GetVectorAxis(vector, VectorAxis::Y)
#define GetZ(vector) GetVectorAxis(vector, VectorAxis::Z)

/// @brief Creates new 3D vector
/// @tparam T Type of number
/// @param x X coordinate
/// @param y Y coordinate
/// @param z Z coordinate
/// @return New 3D vector
template <typename T>
Matrix<T> CreateVector(const T& x, const T& y, const T& z) {
    Array<T> arr = Array<T>((size_t)VectorAxis::AxisCount);
    arr.At(0) = x;
    arr.At(1) = y;
    arr.At(2) = z;
    Matrix<T> ret = Matrix<T>(arr.GetSize(), 1, arr);
    return ret;
}
/// @brief Converts N dimensional vector to N - 1 dimensional vector
/// @tparam T Type of number
/// @param point N vector
/// @param fov Distance on N axis between camera and origin
/// @return N - 1 vector
template <typename T>
Matrix<T> ProjectVector(const Matrix<T>& point, const T& fov = -10) {
    if (point.At(point.GetWidth() - 1, 0) <= (1 + fov)) return CreateVector<T>(MakeNaN(), MakeNaN(), MakeNaN());
    Array<T> arr = Array<T>(point.GetWidth() - 1);
    for (size_t i = 0; i < arr.GetSize(); i++) arr.At(i) = point.At(i, 0);
    Matrix<T> ret = Matrix<T>(point.GetWidth() - 1, 1, arr);
    return ret / (1 - point.At(point.GetWidth() - 1, 0) / fov);
}
template <typename T>
Matrix<T> Reflect(const Matrix<T>& v, const Matrix<T>& n) {
    return v - n * v.Dot(n) * 2;
}
template <typename T>
Matrix<T> Refract(const Matrix<T>& uv, const Matrix<T>& n, const T& t) {
    const Matrix<T> tmp =  (uv + n * Min((-uv).Dot(n), 1)) * t;
    return tmp + n * -Sqrt(Abs(1 - tmp.GetLengthSquared()));
}

#endif