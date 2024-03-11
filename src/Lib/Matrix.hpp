#ifndef Matrix_H
#define Matrix_H
#include "Summation.hpp"
#include <stddef.h>

/// @brief Structure representing mathematic matrixes
/// @tparam T Type of number
/// @tparam width Width of matrix
/// @tparam height Height of matrix
template <typename T, size_t width, size_t height>
struct Matrix {
    /// @brief Returns data at specified position
    /// @param x X position
    /// @param y Y position
    /// @return Data at specified position
    T& At(size_t x, size_t y) {
        return ptr[y * width + x];
    }
    /// @brief Returns data at specified position
    /// @param x X position
    /// @param y Y position
    /// @return Data at specified position
    T At(size_t x, size_t y) const {
        return ptr[y * width + x];
    }
    T GetLength(void) const {
        return std::sqrt(Dot(*this));
    }
    Matrix<T, width, height> Normalize(void) {
        const T len = GetLength();
        Matrix<T, width, height> ret;
        for (size_t y = 0; y < height; y++)
            for (size_t x = 0; x < width; x++) ret.At(x, y) = At(x, y) / len;
        return ret;
    }
    /// @brief a . b = a_0 * b_0 + ... + a_n * b_n
    /// @param other Other matrix
    /// @return Dot product of 2 vectors
    T Dot(Matrix<T, width, height> other) const {
        T ret = 0;
        for (size_t y = 0; y < height; y++)
            for (size_t x = 0; x < width; x++) ret += At(x, y) * other.At(x, y);
        return ret;
    }
    /// @brief Adds 2 matrixes
    /// @param other Other matrix
    /// @return New matrix
    Matrix<T, width, height> operator+(Matrix<T, width, height> other) const {
        Matrix<T, width, height> ret;
        for (size_t y = 0; y < height; y++)
            for (size_t x = 0; x < width; x++) ret.At(x, y) = At(x, y) + other.At(x, y);
        return ret;
    }
    /// @brief Subtracts 2 matrixes
    /// @param other Other matrix
    /// @return New matrix
    Matrix<T, width, height> operator-(Matrix<T, width, height> other) const {
        Matrix<T, width, height> ret;
        for (size_t y = 0; y < height; y++)
            for (size_t x = 0; x < width; x++) ret.At(x, y) = At(x, y) - other.At(x, y);
        return ret;
    }
    /// @brief Multiplies matrix by scalar value
    /// @param scalar Scalar value to multiply by
    /// @return New matrix
    Matrix<T, width, height> operator*(T scalar) const {
        Matrix<T, width, height> ret;
        for (size_t y = 0; y < height; y++)
            for (size_t x = 0; x < width; x++) ret.At(x, y) = At(x, y) * scalar;
        return ret;
    }
    /// @brief Divides matrix by scalar value
    /// @param scalar Scalar value to divide by
    /// @return New matrix
    Matrix<T, width, height> operator/(T scalar) const {
        Matrix<T, width, height> ret;
        for (size_t y = 0; y < height; y++)
            for (size_t x = 0; x < width; x++) ret.At(x, y) = At(x, y) / scalar;
        return ret;
    }
    /// @brief Adds matrix to current matrix
    /// @param other Other matrix
    /// @return New matrix
    Matrix<T, width, height> operator+=(Matrix<T, width, height> other) {
        for (size_t y = 0; y < height; y++)
            for (size_t x = 0; x < width; x++) At(x, y) += other.At(x, y);
        return *this;
    }
    /// @brief Subtracts matrix from current matrix
    /// @param other Other matrix
    /// @return New matrix
    Matrix<T, width, height> operator-=(Matrix<T, width, height> other) {
        for (size_t y = 0; y < height; y++)
            for (size_t x = 0; x < width; x++) At(x, y) -= other.At(x, y);
        return *this;
    }
    /// @brief Multiplies current matrix by scalar value
    /// @param scalar Scalar value
    /// @return New matrix
    Matrix<T, width, height> operator*=(T scalar) {
        for (size_t y = 0; y < height; y++)
            for (size_t x = 0; x < width; x++) At(x, y) *= scalar;
        return *this;
    }
    /// @brief Divides current matrix by scalar value
    /// @param scalar Scalar value
    /// @return New matrix
    Matrix<T, width, height> operator/=(T scalar) {
        for (size_t y = 0; y < height; y++)
            for (size_t x = 0; x < width; x++) At(x, y) /= scalar;
        return *this;
    }
    /// @brief Compares current matrix with another matrix
    /// @param other Other matrix
    /// @return Data equality
    bool operator==(Matrix<T, width, height> other) const {
        bool ret = true;
        for (size_t y = 0; y < height && ret; y++)
            for (size_t x = 0; x < width && ret; x++) ret = At(x, y) == other.At(x, y);
        return ret;
    }
    /// @brief Compares current matrix with another matrix
    /// @param other Other matrix
    /// @return Data unequality
    bool operator!=(Matrix<T, width, height> other) const {
        bool ret = true;
        for (size_t y = 0; y < height && ret; y++)
            for (size_t x = 0; x < width && ret; x++) ret = At(x, y) != other.At(x, y);
        return ret;
    }
    /// @brief Multiplies matrix by -1
    /// @return New matrix
    Matrix<T, width, height> operator-(void) {
        Matrix<T, width, height> ret;
        for (size_t y = 0; y < height; y++)
            for (size_t x = 0; x < width; x++) ret.At(x, y) = -At(x, y);
        return ret;
    }

    private:
    /// @brief Raw data
    T ptr[width * height];
};
/// @brief Converts matrix from one type to another
/// @tparam T Old type of number
/// @tparam F New type of number
/// @tparam width Width of matrix
/// @tparam height Height of matrix
/// @param matrix Matrix to convert
/// @return Converted matrix
template <typename T, typename F, size_t width, size_t height>
Matrix<F, width, height> ConvertMatrix(Matrix<T, width, height> matrix) {
    Matrix<F, width, height> ret;
    for (size_t y = 0; y < height; y++)
        for (size_t x = 0; x < width; x++) ret.At(x, y) = (F)matrix.At(x, y);
    return ret;
}

#endif