#ifndef Matrix_H
#define Matrix_H
#include "Printable.hpp"
#include "Summation.hpp"
#include "Saveable.hpp"
#include "Typedefs.hpp"
#include <vector>

/// @brief Structure representing mathematic matrixes
/// @tparam T Type of number
template <typename T>
struct Matrix : Printable, Saveable {
    /// @brief Creates a new matrix
    constexpr Matrix(size_t w = 0, size_t h = 0) {
        width = w;
        height = h;
        size_t size = width * height;
        ptr.reserve(size);
        for (size_t i = 0; i < size; i++) ptr.push_back(0);
    }
    /// @brief Creates a new matrix
    /// @param arr Values for the matrix
    constexpr Matrix(size_t w, size_t h, std::vector<T> arr) {
        width = w;
        height = h;
        ptr = arr;
    }
    constexpr size_t GetWidth(void) const {
        return width;
    }
    constexpr size_t GetHeight(void) const {
        return height;
    }
    constexpr Matrix<T> GetRow(size_t y) const {
        std::vector<T> ret;
        for (size_t x = 0; x < GetWidth(); x++) ret.push_back(At(x, y));
        return Matrix<T>(GetWidth(), 1, ret);
    }
    constexpr bool IsNan(void) const {
        for (size_t y = 0; y < GetHeight(); y++)
            for (size_t x = 0; x < GetWidth(); x++)
                if (std::isnan(At(x, y))) return true;
        return false;
    }
    constexpr void Fill(T v) {
        for (size_t y = 0; y < GetHeight(); y++)
            for (size_t x = 0; x < GetWidth(); x++) At(x, y) = v;
    }
    void Random(T min, T max) {
        for (size_t y = 0; y < GetHeight(); y++)
            for (size_t x = 0; x < GetWidth(); x++) At(x, y) = RandomNumber(min, max);
    }
    constexpr void Sigmoid(void) {
        for (size_t y = 0; y < GetHeight(); y++)
            for (size_t x = 0; x < GetWidth(); x++) At(x, y) = ::Sigmoid<T>(At(x, y));
    }
    /// @brief Returns data at specified position
    /// @param x X position
    /// @param y Y position
    /// @return Data at specified position
    T& At(size_t x, size_t y) {
        return ptr.at(y * GetWidth() + x);
    }
    /// @brief Returns data at specified position
    /// @param x X position
    /// @param y Y position
    /// @return Data at specified position
    constexpr T At(size_t x, size_t y) const {
        return ptr.at(y * GetWidth() + x);
    }
    /// @brief |a| = sqrt(a . a)
    /// @return Length of the vector
    constexpr T GetLength(void) const {
        return std::sqrt(Dot(*this));
    }
    /// @brief ^a = a / |a|
    /// @return Normalized matrix
    constexpr Matrix<T> Normalize(void) {
        return *this / GetLength();
    }
    /// @brief a . b = a_0 * b_0 + ... + a_n * b_n
    /// @param other Other matrix
    /// @return Dot product of 2 vectors
    constexpr T Dot(Matrix<T> other) const {
        if (other.GetWidth() != GetWidth() || other.GetHeight() != GetHeight()) return NAN;
        T ret = 0;
        for (size_t y = 0; y < GetHeight(); y++)
            for (size_t x = 0; x < GetWidth(); x++) ret += At(x, y) * other.At(x, y);
        return ret;
    }
    /// @brief d = sqrt((b_0 - a_0)^2 + ... + (b_n - a_n)^2)
    /// @param other Other matrix
    /// @return Distance between two matrices
    constexpr T GetDistance(Matrix<T> other) const {
        if (other.GetWidth() != GetWidth() || other.GetHeight() != GetHeight()) return NAN;
        T ret = 0;
        for (size_t y = 0; y < GetHeight(); y++)
            for (size_t x = 0; x < GetWidth(); x++) ret += std::pow(other.At(x, y) - At(x, y), 2);
        return std::sqrt(ret);
    }
    /// @brief Converts matrix to string
    /// @param padding String to pad with
    /// @return String representation of matrix
    virtual std::string ToString(std::string padding = "") const override {
        if (GetHeight() == 1) {
            std::string ret = padding + "[";
            for (size_t x = 0; x < GetWidth(); x++) ret += std::to_string(At(x, 0)) + (((x + 1) == GetWidth()) ? "]" : ", ");
            return ret;
        }
        else {
            std::string ret = padding + "[\n";
            for (size_t y = 0; y < GetHeight(); y++) {
                ret += padding + '\t';
                for (size_t x = 0; x < GetWidth(); x++) ret += std::to_string(At(x, y)) + (((x + 1) == GetWidth() && (y + 1) == GetHeight()) ? "\n" : ", ");
                if ((y + 1) != GetHeight()) ret += '\n';
            }
            return ret + padding + ']';
        }
    }
    /// @brief a + b = [a_0 + b_0, ..., a_n + b_n]
    /// @param other Other matrix
    /// @return New matrix
    constexpr Matrix<T> operator+(Matrix<T> other) const {
        if (other.GetWidth() != GetWidth() || other.GetHeight() != GetHeight()) return Matrix<T>(0, 0);
        Matrix<T> ret = *this;
        for (size_t y = 0; y < GetHeight(); y++)
            for (size_t x = 0; x < GetWidth(); x++) ret.At(x, y) += other.At(x, y);
        return ret;
    }
    /// @brief a - b = [a_0 - b_0, ..., a_n - b_n]
    /// @param other Other matrix
    /// @return New matrix
    constexpr Matrix<T> operator-(Matrix<T> other) const {
        if (other.GetWidth() != GetWidth() || other.GetHeight() != GetHeight()) return Matrix<T>(0, 0);
        Matrix<T> ret = *this;
        for (size_t y = 0; y < GetHeight(); y++)
            for (size_t x = 0; x < GetWidth(); x++) ret.At(x, y) -= other.At(x, y);
        return ret;
    }
    constexpr Matrix<T> operator*(Matrix<T> other) const {
        if (GetWidth() != other.GetHeight()) return Matrix<T>();
        Matrix<T> ret = Matrix<T>(other.GetWidth(), GetHeight());
        for (size_t y = 0; y < ret.GetHeight(); y++) {
            for (size_t x = 0; x < ret.GetWidth(); x++) {
                ret.At(x, y) = 0;
                for (size_t x2 = 0; x2 < GetWidth(); x2++) ret.At(x, y) += At(x2, y) * other.At(x, x2);
            }
        }
        return ret;
    }
    /// @brief a * s = [a_0 * s, ..., a_n * s]
    /// @param scalar Scalar value to multiply by
    /// @return New matrix
    constexpr Matrix<T> operator*(T scalar) const {
        Matrix<T> ret = *this;
        for (size_t y = 0; y < GetHeight(); y++)
            for (size_t x = 0; x < GetWidth(); x++) ret.At(x, y) *= scalar;
        return ret;
    }
    /// @brief a / s = [a_0 / s, ..., a_n / s]
    /// @param scalar Scalar value to divide by
    /// @return New matrix
    constexpr Matrix<T> operator/(T scalar) const {
        Matrix<T> ret = *this;
        for (size_t y = 0; y < GetHeight(); y++)
            for (size_t x = 0; x < GetWidth(); x++) ret.At(x, y) /= scalar;
        return ret;
    }
    /// @brief a + b = [a_0 + b_0, ..., a_n + b_n]
    /// @param other Other matrix
    /// @return New matrix
    constexpr Matrix<T> operator+=(Matrix<T> other) {
        if (other.GetWidth() != GetWidth() || other.GetHeight() != GetHeight()) return Matrix<T>(0, 0);
        for (size_t y = 0; y < GetHeight(); y++)
            for (size_t x = 0; x < GetWidth(); x++) At(x, y) += other.At(x, y);
        return *this;
    }
    /// @brief a - b = [a_0 - b_0, ..., a_n - b_n]
    /// @param other Other matrix
    /// @return New matrix
    constexpr Matrix<T> operator-=(Matrix<T> other) {
        if (other.GetWidth() != GetWidth() || other.GetHeight() != GetHeight()) return Matrix<T>(0, 0);
        for (size_t y = 0; y < GetHeight(); y++)
            for (size_t x = 0; x < GetWidth(); x++) At(x, y) -= other.At(x, y);
        return *this;
    }
    /// @brief a * s = [a_0 * s, ..., a_n * s]
    /// @param scalar Scalar value
    /// @return New matrix
    constexpr Matrix<T> operator*=(T scalar) {
        for (size_t y = 0; y < GetHeight(); y++)
            for (size_t x = 0; x < GetWidth(); x++) At(x, y) *= scalar;
        return *this;
    }
    /// @brief a / s = [a_0 / s, ..., a_n / s]
    /// @param scalar Scalar value
    /// @return New matrix
    constexpr Matrix<T> operator/=(T scalar) {
        for (size_t y = 0; y < GetHeight(); y++)
            for (size_t x = 0; x < GetWidth(); x++) At(x, y) /= scalar;
        return *this;
    }
    /// @brief Compares current matrix with another matrix
    /// @param other Other matrix
    /// @return Data equality
    constexpr bool operator==(Matrix<T> other) const {
        bool ret = other.GetWidth() == GetWidth() && other.GetHeight() == GetHeight();
        for (size_t y = 0; y < GetHeight() && ret; y++)
            for (size_t x = 0; x < GetWidth() && ret; x++) ret = At(x, y) == other.At(x, y);
        return ret;
    }
    /// @brief Compares current matrix with another matrix
    /// @param other Other matrix
    /// @return Data unequality
    constexpr bool operator!=(Matrix<T> other) const {
        return !(*this == other);
    }
    /// @brief -a = [-a_0, ..., -a_n]
    /// @return New matrix
    constexpr Matrix<T> operator-(void) {
        return *this * -1;
    }
    virtual bool Save(FILE* file) const override {
        if (fwrite(&width, sizeof(size_t), 1, file) != 1) return false;
        if (fwrite(&height, sizeof(size_t), 1, file) != 1) return false;
        size_t size = width * height;
        if (fwrite(ptr.data(), sizeof(T), size, file) != size) return false;
        return true;
    }
    virtual bool Load(FILE* file) override {
        if (fread(&width, sizeof(size_t), 1, file) != 1) return false;
        if (fread(&height, sizeof(size_t), 1, file) != 1) return false;
        size_t size = width * height;
        ptr = {};
        ptr.reserve(size);
        for (size_t i = 0; i < width * height; i++) ptr.push_back(0);
        if (fread(ptr.data(), sizeof(T), size, file) != size) return false;
        return true;
    }

    // column -> x
    // row -> y
    private:
    /// @brief Raw data
    std::vector<T> ptr;
    size_t width;
    size_t height;
};
/// @brief Converts matrix from one type to another
/// @tparam T Old type of number
/// @tparam F New type of number
/// @tparam width Width of matrix
/// @tparam height Height of matrix
/// @param matrix Matrix to convert
/// @return Converted matrix
template <typename T, typename F>
Matrix<F> ConvertMatrix(Matrix<T> matrix) {
    Matrix<F> ret = Matrix<F>(matrix.GetWidth(), matrix.GetHeight());
    for (size_t y = 0; y < matrix.GetHeight(); y++)
        for (size_t x = 0; x < matrix.GetWidth(); x++) ret.At(x, y) = (F)matrix.At(x, y);
    return ret;
}

#endif