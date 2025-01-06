#ifndef MathLib_Vector_H
#define MathLib_Vector_H
#include "Matrix.hpp"
#include "../MinMax.hpp"

namespace MathLib {
    /// @brief Representation of axis position in matrix
    enum class VectorAxis {
        X = 0,
        Y,
        Z,
        AxisCount,
    };

    #define GetVectorAxis(vector, axis) (vector).At((size_t)(axis), 0)
    #define GetX(vector) GetVectorAxis(vector, MathLib::VectorAxis::X)
    #define GetY(vector) GetVectorAxis(vector, MathLib::VectorAxis::Y)
    #define GetZ(vector) GetVectorAxis(vector, MathLib::VectorAxis::Z)

    /// @brief Creates new 3D vector
    /// @tparam T Type of number
    /// @param x X coordinate
    /// @param y Y coordinate
    /// @param z Z coordinate
    /// @return New 3D vector
    template <typename T>
    [[nodiscard]] Matrix<T> CreateVector(const T& x, const T& y, const T& z) {
        StartBenchmark
        Array<T> arr = Array<T>((size_t)VectorAxis::AxisCount);
        arr.At(0) = x;
        arr.At(1) = y;
        arr.At(2) = z;
        ReturnFromBenchmark(Matrix<T>(arr.GetSize(), 1, arr));
    }
    /// @brief Converts N dimensional vector to N - 1 dimensional vector
    /// @tparam T Type of number
    /// @param point N vector
    /// @param fov Distance on N axis between camera and origin
    /// @return N - 1 vector
    template <typename T>
    [[nodiscard]] Matrix<T> ProjectVector(const Matrix<T>& point, const T& fov = -10) {
        StartBenchmark
        if (point.At(point.GetWidth() - 1, 0) <= (1 + fov)) ReturnFromBenchmark(CreateVector<T>(MakeNaN(), MakeNaN(), MakeNaN()));
        Array<T> arr = Array<T>(point.GetWidth() - 1);
        for (size_t i = 0; i < arr.GetSize(); i++) arr.At(i) = point.At(i, 0);
        ReturnFromBenchmark(Matrix<T>(point.GetWidth() - 1, 1, arr) / (1 - point.At(point.GetWidth() - 1, 0) / fov));
    }
    /// @brief Converts N dimensional vector to M dimensional vector
    /// @tparam T Type of number
    /// @param point N vector
    /// @param fov Distance on N axis between camera and origin
    /// @return M vector
    template <typename T>
    [[nodiscard]] Matrix<T> ConvertVectorDimension(const Matrix<T>& point, size_t dimension, const T& fov = -10) {
        StartBenchmark
        if (point.GetWidth() < dimension) ReturnFromBenchmark(CreateVector<T>(MakeNaN(), MakeNaN(), MakeNaN()));
        Matrix<T> pos = point;
        while (pos.GetWidth() != dimension) pos = ProjectVector<T>(pos, fov);
        ReturnFromBenchmark(pos);
    }
    template <typename T>
    [[nodiscard]] Matrix<T> Reflect(const Matrix<T>& v, const Matrix<T>& n) {
        StartBenchmark
        ReturnFromBenchmark(v - n * v.Dot(n) * 2);
    }
    template <typename T>
    [[nodiscard]] Matrix<T> Refract(const Matrix<T>& uv, const Matrix<T>& n, const T& t) {
        StartBenchmark
        const Matrix<T> tmp =  (uv + n * Min<T>((-uv).Dot(n), 1)) * t;
        ReturnFromBenchmark(tmp + n * -Sqrt(Abs(1 - tmp.GetLengthSquared())));
    }
}

#endif