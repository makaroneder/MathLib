#ifndef Matrix_H
#define Matrix_H
#include "MathObject.hpp"
#include "Factorial.hpp"
#include "Printable.hpp"
#include "Saveable.hpp"
#include "Host.hpp"

/// @brief Structure representing mathematic matrixes
/// @tparam T Type of number
template <typename T>
struct Matrix : Printable, Saveable {
    CreateOperators(Matrix<T>, T)
    /// @brief Creates a new matrix
    /// @param w Width of matrix
    /// @param h Height of matrix
    constexpr Matrix(const size_t& w = 0, const size_t& h = 0) : width(w), height(h), ptr(Array<T>(w * h)) {
        for (T& val : ptr) val = 0;
    }
    /// @brief Creates a new matrix
    /// @param w Width of matrix
    /// @param h Height of matrix
    /// @param arr Values for the matrix
    constexpr Matrix(const size_t& w, const size_t& h, const Array<T>& arr) : width(w), height(h), ptr(arr) {}
    static constexpr Matrix<T> Identity(size_t n) {
        Matrix<T> ret = Matrix<T>(n, n);
        for (size_t i = 0; i < n; i++) ret.At(i, i) = 1;
        return ret;
    }
    constexpr size_t GetWidth(void) const {
        return width;
    }
    constexpr size_t GetHeight(void) const {
        return height;
    }
    constexpr Array<T> GetValue(void) const {
        return ptr;
    }
    constexpr Matrix<T> GetRow(const size_t& y) const {
        Array<T> ret = Array<T>(width);
        for (size_t x = 0; x < width; x++) ret.At(x) = At(x, y);
        return Matrix<T>(width, 1, ret);
    }
    constexpr void Fill(const T& v) {
        for (T& val : ptr) val = v;
    }
    void Random(const T& min, const T& max) {
        for (T& val : ptr) val = RandomNumber(min, max);
    }
    /// @brief Returns data at specified position
    /// @param x X position
    /// @param y Y position
    /// @return Data at specified position
    T& At(const size_t& x, const size_t& y) {
        return ptr.At(y * width + x);
    }
    /// @brief Returns data at specified position
    /// @param x X position
    /// @param y Y position
    /// @return Data at specified position
    constexpr T At(const size_t& x, const size_t& y) const {
        return ptr.At(y * width + x);
    }
    /// @brief a = (a . b) / (|a| * |b|)
    /// @param other Other matrix
    /// @return Angle between 2 matrices
    constexpr T GetAngle(const Matrix<T>& other) const {
        return Dot(other) / (GetLength() * other.GetLength());
    }
    /// @brief |a|^2 = a . a
    /// @return Squared length of the vector
    constexpr T GetLengthSquared(void) const {
        return Dot(*this);
    }
    /// @brief |a| = sqrt(a . a)
    /// @return Length of the vector
    constexpr T GetLength(void) const {
        return Sqrt(GetLengthSquared());
    }
    /// @brief ^a = a / |a|
    /// @return Normalized matrix
    constexpr Matrix<T> Normalize(void) const {
        const T len = GetLength();
        return FloatsEqual<T>(len, 0) ? *this : (*this / len);
    }
    /// @brief a . b = a_0 * b_0 + ... + a_n * b_n
    /// @param other Other matrix
    /// @return Dot product of 2 vectors
    constexpr T Dot(const Matrix<T>& other) const {
        if (other.width != width || other.height != height) return MakeNaN();
        T ret = 0;
        for (size_t y = 0; y < height; y++)
            for (size_t x = 0; x < width; x++) ret += At(x, y) * other.At(x, y);
        return ret;
    }
    /// @brief ln(A) = (-1)^(1 + 1) * ((A - I)^1 / 1) + ... + (-1)^(1 + ∞) * ((A - I)^∞ / ∞)
    /// @return Logarithm of matrix
    Matrix<T> Log(void) const {
        const Matrix<T> identity = Identity(width);
        Matrix<T> ret = Matrix<T>(width, height);
        for (T k = 1; k < 100; k++) ret += ((*this - identity).UnsignedPow(k) / k) * Pow(-1, k + 1);
        return ret;
    }
    /// @brief e^X = X^0 / 0! + ... + X^∞ / ∞!
    /// @return Exponential of matrix
    Matrix<T> Exponential(void) const {
        Matrix<T> ret = Matrix<T>(width, height);
        for (size_t k = 0; k < 100; k++) ret += UnsignedPow(k) / Factorial<T>(k, 1);
        return ret;
    }
    /// @brief X^n = exp(ln(X) * n)
    /// @param n Exponent
    /// @return Power of matrix
    Matrix<T> Pow(T n) const {
        if (n < 0) return GetInverse().Pow(Abs(n));
        return (Log() * n).Exponential();
    }
    /// @brief X^n = exp(ln(X) * n)
    /// @param n Exponent matrix
    /// @return Power of matrix
    Matrix<T> Pow(Matrix<T> n) const {
        return (Log() * n).Exponential();
    }
    /// @brief Checks whether the matrix is multiple of another matrix
    /// @param other Another matrix
    /// @return Check status
    bool IsMultipleOf(const Matrix<T>& other) const {
        if (other.width != width || other.height != height) return false;
        T prev = 0;
        for (size_t y = 0; y < height; y++) {
            for (size_t x = 0; x < width; x++) {
                if (FloatsEqual<T>(At(x, y), 0) && FloatsEqual<T>(other.At(x, y), 0)) continue;
                const T div = At(x, y) / other.At(x, y);
                if (FloatsEqual<T>(prev, 0)) prev = div;
                if (!FloatsEqual<T>(prev, div)) return false;
            }
        }
        return true;
    }
    /// @brief Returns determinant of the matrix
    /// @return Determinant of the matrix
    T GetDeterminant(void) const {
        if (width != height) return MakeNaN();
        const size_t dimension = width;
        if (dimension == 0) return 1;
        if (dimension == 1) return At(0, 0);
        if (dimension == 2) return At(0, 0) * At(1, 1) - At(0, 1) * At(1, 0);
        T ret = 0;
        int sign = 1;
        for (size_t i = 0; i < dimension; i++) {
            Matrix<T> sub = Matrix<T>(dimension - 1, dimension - 1);
            for (size_t m = 1; m < dimension; m++) {
                size_t z = 0;
                for (size_t n = 0; n < dimension; n++) {
                    if (n != i) {
                        sub.At(m - 1, z) = At(m, n);
                        z++;
                    }
                }
            }
            ret += sign * At(0, i) * sub.GetDeterminant();
            sign = -sign;
        }
        return ret;
    }
    /// @brief Returns transpose of the matrix
    /// @return Transpose of the matrix
    Matrix<T> GetTranspose(void) const {
        Matrix<T> ret = Matrix<T>(height, width);
        for (size_t y = 0; y < height; y++)
            for (size_t x = 0; x < width; x++) ret.At(y, x) = At(x, y);
        return ret;
    }
    /// @brief Returns cofactor of the matrix
    /// @return Cofactor of the matrix
    Matrix<T> GetCofactor(void) const {
        if (width != height) return Matrix<T>(0, 0);
        Matrix<T> ret = Matrix<T>(width, width);
        Matrix<T> sub = Matrix<T>(width - 1, width - 1);
        for (size_t i = 0; i < width; i++) {
            for (size_t j = 0; j < width; j++) {
                size_t p = 0;
                for (size_t x = 0; x < width; x++) {
                    if (x == i) continue;
                    size_t q = 0;
                    for (size_t y = 0; y < width; y++) {
                        if (y == j) continue;
                        sub.At(p, q) = At(x, y);
                        q++;
                    }
                    p++;
                }
                ret.At(i, j) = ::Pow(-1, i + j) * sub.GetDeterminant();
            }
        }
        return ret;
    }
    /// @brief Returns inverse of the matrix
    /// @return Inverse of the matrix
    Matrix<T> GetInverse(void) const {
        if (GetDeterminant() == 0) return Matrix<T>(0, 0);
        return GetCofactor().GetTranspose() / GetDeterminant();
    }
    /// @brief Converts matrix to string
    /// @param padding String to pad with
    /// @return String representation of matrix
    virtual String ToString(const String& padding = "") const override {
        if (height == 1) {
            String ret = padding + "[";
            for (size_t x = 0; x < width; x++) ret += ::ToString(At(x, 0)) + (((x + 1) == width) ? "]" : ", ");
            return ret;
        }
        else {
            String ret = padding + "[\n";
            for (size_t y = 0; y < height; y++) {
                ret += padding + '\t';
                for (size_t x = 0; x < width; x++) ret += ::ToString(At(x, y)) + (((x + 1) == width && (y + 1) == height) ? "\n" : ", ");
                if ((y + 1) != height) ret += '\n';
            }
            return ret + padding + ']';
        }
    }
    /// @brief Multiplies 2 matrices
    /// @param other Another matrix
    /// @return Result of multiplication
    constexpr Matrix<T> operator*(const Matrix<T>& other) const {
        if (width != other.height) return Matrix<T>();
        Matrix<T> ret = Matrix<T>(other.width, height);
        for (size_t y = 0; y < ret.height; y++) {
            for (size_t x = 0; x < ret.width; x++) {
                ret.At(x, y) = 0;
                for (size_t x2 = 0; x2 < width; x2++) ret.At(x, y) += At(x2, y) * other.At(x, x2);
            }
        }
        return ret;
    }
    /// @brief Compares current matrix with another matrix
    /// @param other Other matrix
    /// @return Data equality
    constexpr bool operator==(const Matrix<T>& other) const {
        bool ret = other.width == width && other.height == height;
        for (size_t y = 0; y < height && ret; y++)
            for (size_t x = 0; x < width && ret; x++) ret = At(x, y) == other.At(x, y);
        return ret;
    }
    /// @brief Saves matrix data
    /// @param file File to save matrix data into
    /// @return Status
    virtual bool Save(ByteDevice& file) const override {
        if (!file.Write<size_t>(width) || !file.Write<size_t>(height)) return false;
        const size_t size = ptr.GetSize();
        for (size_t i = 0; i < size; i++) {
            const T tmp = ptr.At(i);
            if (!file.Write<T>(tmp)) return false;
        }
        return true;
    }
    /// @brief Loads matrix data
    /// @param file File to load matrix data from
    /// @return Status
    virtual bool Load(ByteDevice& file) override {
        if (!file.Read<size_t>(width) || !file.Read<size_t>(height)) return false;
        ptr = Array<T>(width * height);
        const size_t size = ptr.GetSize();
        for (size_t i = 0; i < size; i++) {
            T tmp;
            if (!file.Read<T>(tmp)) return false;
            ptr.At(i) = tmp;
        }
        return true;
    }

    private:
    /// @brief a + b = [a_0 + b_0, ..., a_n + b_n]
    /// @param other Matrix to add
    void Add(const Matrix<T>& other) {
        if (other.width != width || other.height != height) Panic("Invalid width or height of matrixes for addition");
        for (size_t y = 0; y < height; y++)
            for (size_t x = 0; x < width; x++) At(x, y) += other.At(x, y);
    }
    /// @brief a * s = [a_0 * s, ..., a_n * s]
    /// @param scalar Scalar value to multiply by
    void Multiply(const T& scalar) {
        for (size_t y = 0; y < height; y++)
            for (size_t x = 0; x < width; x++) At(x, y) *= scalar;
    }
    /// @brief X^n = X * ... * X
    /// @param n Exponent
    /// @return Unsigned power of matrix
    Matrix<T> UnsignedPow(const size_t& n) const {
        if (width != height) Panic("Matrix is not quadratic");
        if (n == 0) return Matrix<T>::Identity(width);
        Matrix<T> ret = *this;
        for (size_t i = 1; i < n; i++) ret = ret * *this;
        return ret;
    }

    /// @brief Width of matrix
    size_t width;
    /// @brief Height of matrix
    size_t height;
    /// @brief Raw data
    Array<T> ptr;
};
/// @brief Converts matrix from one type to another
/// @tparam T Old type of number
/// @tparam F New type of number
/// @tparam width Width of matrix
/// @tparam height Height of matrix
/// @param matrix Matrix to convert
/// @return Converted matrix
template <typename T, typename F>
Matrix<F> ConvertMatrix(const Matrix<T>& matrix) {
    Matrix<F> ret = Matrix<F>(matrix.GetWidth(), matrix.GetHeight());
    for (size_t y = 0; y < matrix.GetHeight(); y++)
        for (size_t x = 0; x < matrix.GetWidth(); x++) ret.At(x, y) = (F)matrix.At(x, y);
    return ret;
}

#endif