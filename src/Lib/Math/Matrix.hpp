#ifndef MathLib_Math_Matrix_H
#define MathLib_Math_Matrix_H
#include "../Interfaces/Printable.hpp"
#include "../Interfaces/Function.hpp"
#include "../Interfaces/Saveable.hpp"
#include "Exponential.hpp"
#include "MathObject.hpp"
#include "Factorial.hpp"
#include "Interval.hpp"
#include "../Host.hpp"

namespace MathLib {
    /// @brief Structure representing mathematic matrixes
    /// @tparam T Type of number
    template <typename T>
    struct Matrix : Iteratable<T>, Printable, Saveable {
        CreateOperators(Matrix<T>, T)
        CreateExponential(Matrix<T>, IsSquare(), Identity(width))
        /// @brief Creates a new matrix
        /// @param width Width of matrix
        /// @param height Height of matrix
        Matrix(size_t width = 0, size_t height = 0) : width(width), height(height), ptr(width * height) {
            StartBenchmark
            Fill(T());
            EndBenchmark
        }
        /// @brief Creates a new matrix
        /// @param width Width of matrix
        /// @param height Height of matrix
        /// @param func Function representing the matrix
        Matrix(size_t width, size_t height, const Function<T, size_t, size_t>& func) : width(width), height(height), ptr(width * height) {
            StartBenchmark
            for (size_t y = 0; y < height; y++)
                for (size_t x = 0; x < width; x++) At(x, y) = func(x, y);
            EndBenchmark
        }
        /// @brief Creates a new matrix
        /// @param width Width of matrix
        /// @param height Height of matrix
        /// @param arr Values for the matrix
        Matrix(size_t width, size_t height, const Sequence<T>& arr) : width(width), height(height), ptr(CollectionToArray<T>(arr)) {
            EmptyBenchmark
        }
        [[nodiscard]] static Matrix<T> Identity(size_t n) {
            StartBenchmark
            Matrix<T> ret = Matrix<T>(n, n);
            for (size_t i = 0; i < n; i++) ret.At(i, i) = 1;
            ReturnFromBenchmark(ret);
        }
        [[nodiscard]] size_t GetWidth(void) const {
            StartAndReturnFromBenchmark(width);
        }
        [[nodiscard]] size_t GetHeight(void) const {
            StartAndReturnFromBenchmark(height);
        }
        [[nodiscard]] Array<T> GetValue(void) const {
            StartAndReturnFromBenchmark(ptr);
        }
        [[nodiscard]] Matrix<T> GetRow(size_t y) const {
            StartBenchmark
            Array<T> ret = Array<T>(width);
            for (size_t x = 0; x < width; x++) ret.At(x) = At(x, y);
            ReturnFromBenchmark(Matrix<T>(width, 1, ret));
        }
        [[nodiscard]] bool IsSquare(void) const {
            StartBenchmark
            ReturnFromBenchmark(width == height);
        }
        [[nodiscard]] bool IsInside(const Matrix<Interval<T>>& interval) const {
            StartBenchmark
            if (GetWidth() != interval.GetWidth() || GetHeight() != interval.GetHeight()) ReturnFromBenchmark(false);
            for (size_t y = 0; y < GetHeight(); y++)
                for (size_t x = 0; x < GetHeight(); x++)
                    if (!interval.At(x, y).Contains(At(x, y))) ReturnFromBenchmark(false);
            ReturnFromBenchmark(true);
        }
        [[nodiscard]] bool Swap(const Interval<size_t>& xInterval1, const Interval<size_t>& yInterval1, const Interval<size_t>& xInterval2, const Interval<size_t>& yInterval2) {
            StartBenchmark
            if (xInterval1.GetSize() != xInterval2.GetSize() || yInterval1.GetSize() != yInterval2.GetSize()) ReturnFromBenchmark(false);
            for (size_t y = 0; y < yInterval1.GetSize(); y++)
                for (size_t x = 0; x < xInterval1.GetSize(); x++) MathLib::Swap<T>(At(x + xInterval1.GetMin(), y + yInterval1.GetMin()), At(x + xInterval2.GetMin(), y + yInterval2.GetMin()));
            ReturnFromBenchmark(true);
        }
        [[nodiscard]] bool Add(const T& multiple, const Interval<size_t>& xInterval1, const Interval<size_t>& yInterval1, const Interval<size_t>& xInterval2, const Interval<size_t>& yInterval2) {
            StartBenchmark
            if (xInterval1.GetSize() != xInterval2.GetSize() || yInterval1.GetSize() != yInterval2.GetSize()) ReturnFromBenchmark(false);
            for (size_t y = 0; y < yInterval1.GetSize(); y++)
                for (size_t x = 0; x < xInterval1.GetSize(); x++) At(x + xInterval1.GetMin(), y + yInterval1.GetMin()) += At(x + xInterval2.GetMin(), y + yInterval2.GetMin()) * multiple;
            ReturnFromBenchmark(true);
        }
        void Multiply(const T& multiple, const Interval<size_t>& xInterval, const Interval<size_t>& yInterval) {
            StartBenchmark
            for (size_t y = yInterval.GetMin(); y < yInterval.GetMax(); y++)
                for (size_t x = xInterval.GetMin(); x < xInterval.GetMax(); x++) At(x, y) *= multiple;
            EndBenchmark;
        }
        [[nodiscard]] bool Insert(const Matrix<T>& other, const Interval<size_t>& xInterval1, const Interval<size_t>& yInterval1, const Interval<size_t>& xInterval2, const Interval<size_t>& yInterval2) {
            StartBenchmark
            if (xInterval1.GetSize() != xInterval2.GetSize() || yInterval1.GetSize() != yInterval2.GetSize()) ReturnFromBenchmark(false);
            for (size_t y = 0; y < yInterval1.GetSize(); y++)
                for (size_t x = 0; x < xInterval1.GetSize(); x++) At(x + xInterval1.GetMin(), y + yInterval1.GetMin()) = other.At(x + xInterval2.GetMin(), y + yInterval2.GetMin());
            ReturnFromBenchmark(true);
        }
        [[nodiscard]] bool IsFilledWith(const T& value, const Interval<size_t>& xInterval, const Interval<size_t>& yInterval) const {
            StartBenchmark
            for (size_t y = yInterval.GetMin(); y < yInterval.GetMax(); y++)
                for (size_t x = xInterval.GetMin(); x < xInterval.GetMax(); x++)
                    if (!MathLib::FloatsEqual<T>(At(x, y), value)) ReturnFromBenchmark(false);
            ReturnFromBenchmark(true);
        }
        /// @brief Checks whether the matrix is multiple of another matrix
        /// @param other Another matrix
        /// @return Check status
        [[nodiscard]] bool IsMultipleOf(const Matrix<T>& other, const Interval<size_t>& xInterval1, const Interval<size_t>& yInterval1, const Interval<size_t>& xInterval2, const Interval<size_t>& yInterval2) const {
            StartBenchmark
            if (xInterval1.GetSize() != xInterval2.GetSize() || yInterval1.GetSize() != yInterval2.GetSize()) ReturnFromBenchmark(false);
            T prev = 0;
            for (size_t y = 0; y < yInterval1.GetSize(); y++) {
                for (size_t x = 0; x < xInterval1.GetSize(); x++) {
                    const T v1 = At(x + xInterval1.GetMin(), y + yInterval1.GetMin());
                    const T v2 = other.At(x + xInterval2.GetMin(), y + yInterval2.GetMin());
                    if (FloatsEqual<T>(v1, 0) && FloatsEqual<T>(v2, 0)) continue;
                    const T div = v1 / v2;
                    if (FloatsEqual<T>(prev, 0)) prev = div;
                    if (!FloatsEqual<T>(prev, div)) ReturnFromBenchmark(false);
                }
            }
            ReturnFromBenchmark(true);
        }
        [[nodiscard]] bool IsMultipleOf(const Interval<size_t>& xInterval1, const Interval<size_t>& yInterval1, const Interval<size_t>& xInterval2, const Interval<size_t>& yInterval2) const {
            StartBenchmark
            if (xInterval1.GetSize() != xInterval2.GetSize() || yInterval1.GetSize() != yInterval2.GetSize()) ReturnFromBenchmark(false);
            T prev = 0;
            for (size_t y = 0; y < yInterval1.GetSize(); y++) {
                for (size_t x = 0; x < xInterval1.GetSize(); x++) {
                    const T v1 = At(xInterval1.GetMin() + x, yInterval1.GetMin() + y);
                    const T v2 = At(xInterval2.GetMin() + x, yInterval2.GetMin() + y);
                    if (FloatsEqual<T>(v1, 0) && FloatsEqual<T>(v2, 0)) continue;
                    const T div = v1 / v2;
                    if (FloatsEqual<T>(prev, 0)) prev = div;
                    if (!FloatsEqual<T>(prev, div)) ReturnFromBenchmark(false);
                }
            }
            ReturnFromBenchmark(true);
        }
        void Fill(const T& v) {
            StartBenchmark
            for (T& val : ptr) val = v;
            EndBenchmark
        }
        void Random(const T& min, const T& max) {
            StartBenchmark
            for (T& val : ptr) val = RandomNumber(min, max);
            EndBenchmark
        }
        /// @brief Returns data at specified position
        /// @param x X position
        /// @param y Y position
        /// @return Data at specified position
        [[nodiscard]] T& At(size_t x, size_t y) {
            StartAndReturnFromBenchmark(ptr.At(y * width + x));
        }
        /// @brief Returns data at specified position
        /// @param x X position
        /// @param y Y position
        /// @return Data at specified position
        [[nodiscard]] T At(size_t x, size_t y) const {
            StartAndReturnFromBenchmark(ptr.At(y * width + x));
        }
        [[nodiscard]] T GetSum(void) const {
            T ret = 0;
            for (const T& val : ptr) ret += val;
            return ret;
        }
        [[nodiscard]] Expected<Matrix<T>> HadamardProduct(const Matrix<T>& other) const {
            StartBenchmark
            if (width != other.GetWidth() || height != other.GetHeight()) ReturnFromBenchmark(Expected<Matrix<T>>());
            Matrix<T> ret = *this;
            for (size_t y = 0; y < height; y++)
                for (size_t x = 0; x < width; x++) ret.At(x, y) *= other.At(x, y);
            ReturnFromBenchmark(ret);
        }
        /// @brief a = (a . b) / (|a| * |b|)
        /// @param other Other matrix
        /// @return Angle between 2 matrices
        [[nodiscard]] T GetAngle(const Matrix<T>& other) const {
            StartAndReturnFromBenchmark(Dot(other) / (GetLength() * other.GetLength()));
        }
        /// @brief |a|^2 = a . a
        /// @return Squared length of the vector
        [[nodiscard]] T GetLengthSquared(void) const {
            StartAndReturnFromBenchmark(Dot(*this));
        }
        /// @brief |a| = sqrt(a . a)
        /// @return Length of the vector
        [[nodiscard]] T GetLength(void) const {
            StartAndReturnFromBenchmark(Sqrt(GetLengthSquared()));
        }
        /// @brief ^a = a / |a|
        /// @return Normalized matrix
        [[nodiscard]] Matrix<T> Normalize(void) const {
            StartBenchmark
            const T len = GetLength();
            ReturnFromBenchmark(FloatsEqual<T>(len, 0) ? *this : (*this / len));
        }
        /// @brief a . b = a_0 * b_0 + ... + a_n * b_n
        /// @param other Other matrix
        /// @return Dot product of 2 vectors
        [[nodiscard]] T Dot(const Matrix<T>& other) const {
            StartBenchmark
            if (other.width != width || other.height != height) ReturnFromBenchmark(nan);
            T ret = 0;
            for (size_t y = 0; y < height; y++)
                for (size_t x = 0; x < width; x++) ret += At(x, y) * other.At(x, y);
            ReturnFromBenchmark(ret);
        }
        /// @brief ln(A) = (-1)^(1 + 1) * ((A - I)^1 / 1) + ... + (-1)^(1 + ∞) * ((A - I)^∞ / ∞)
        /// @return Logarithm of matrix
        [[nodiscard]] Expected<Matrix<T>> Log(void) const {
            StartBenchmark
            const Matrix<T> identity = Identity(width);
            Matrix<T> ret = Matrix<T>(width, height);
            for (T k = 1; k < 100; k++) {
                const Expected<Matrix<T>> tmp = (*this - identity).UnsignedPow(k);
                if (!tmp.HasValue()) ReturnFromBenchmark(Expected<Matrix<T>>())
                else ret += tmp.Get() * Pow(-1, k + 1) / k;
            }
            ReturnFromBenchmark(Expected<Matrix<T>>(ret));
        }
        /// @brief e^X = X^0 / 0! + ... + X^∞ / ∞!
        /// @return Exponential of matrix
        [[nodiscard]] Expected<Matrix<T>> Exponential(void) const {
            StartBenchmark
            Matrix<T> ret = Matrix<T>(width, height);
            for (size_t k = 0; k < 100; k++) {
                const Expected<Matrix<T>> tmp = UnsignedPow(k);
                if (!tmp.HasValue()) ReturnFromBenchmark(Expected<Matrix<T>>())
                else ret += tmp.Get() / Factorial<T>(k, 1);
            }
            ReturnFromBenchmark(Expected<Matrix<T>>(ret));
        }
        /// @brief X^n = exp(ln(X) * n)
        /// @param n Exponent
        /// @return Power of matrix
        [[nodiscard]] Expected<Matrix<T>> Pow(T n) const {
            StartBenchmark
            if (n < 0) {
                const Expected<Matrix<T>> tmp = GetInverse();
                ReturnFromBenchmark(tmp.HasValue() ? tmp.Get().Pow(-n) : Expected<Matrix<T>>());
            }
            else {
                const Expected<Matrix<T>> tmp = Log();
                ReturnFromBenchmark(tmp.HasValue() ? (tmp.Get() * n).Exponential() : Expected<Matrix<T>>());
            }
        }
        /// @brief X^n = exp(ln(X) * n)
        /// @param n Exponent matrix
        /// @return Power of matrix
        [[nodiscard]] Expected<Matrix<T>> Pow(Matrix<T> n) const {
            StartBenchmark
            const Expected<Matrix<T>> tmp = Log();
            ReturnFromBenchmark(tmp.HasValue() ? (tmp.Get() * n).Exponential() : Expected<Matrix<T>>());
        }
        /// @brief Returns determinant of the matrix
        /// @return Determinant of the matrix
        [[nodiscard]] T GetDeterminant(void) const {
            StartBenchmark
            if (!IsSquare()) ReturnFromBenchmark(nan)
            else if (!width) ReturnFromBenchmark(1)
            else if (width == 1) ReturnFromBenchmark(At(0, 0))
            else if (width == 2) ReturnFromBenchmark(At(0, 0) * At(1, 1) - At(0, 1) * At(1, 0))
            T ret = 0;
            int sign = 1;
            for (size_t i = 0; i < width; i++) {
                Matrix<T> sub = Matrix<T>(width - 1, width - 1);
                for (size_t m = 1; m < width; m++) {
                    size_t z = 0;
                    for (size_t n = 0; n < width; n++)
                        if (n != i) sub.At(m - 1, z++) = At(m, n);
                }
                ret += sign * At(0, i) * sub.GetDeterminant();
                sign *= -1;
            }
            ReturnFromBenchmark(ret);
        }
        /// @brief Returns transpose of the matrix
        /// @return Transpose of the matrix
        [[nodiscard]] Matrix<T> GetTranspose(void) const {
            StartBenchmark
            Matrix<T> ret = Matrix<T>(height, width);
            for (size_t y = 0; y < height; y++)
                for (size_t x = 0; x < width; x++) ret.At(y, x) = At(x, y);
            ReturnFromBenchmark(ret);
        }
        /// @brief Returns cofactor of the matrix
        /// @return Cofactor of the matrix
        [[nodiscard]] Expected<Matrix<T>> GetCofactor(void) const {
            StartBenchmark
            if (!IsSquare()) ReturnFromBenchmark(Expected<Matrix<T>>());
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
                    ret.At(i, j) = MathLib::Pow(-1, i + j) * sub.GetDeterminant();
                }
            }
            ReturnFromBenchmark(Expected<Matrix<T>>(ret));
        }
        /// @brief Returns inverse of the matrix
        /// @return Inverse of the matrix
        [[nodiscard]] Expected<Matrix<T>> GetInverse(void) const {
            StartBenchmark
            const Expected<Matrix<T>> tmp = GetCofactor();
            const T determinant = GetDeterminant();
            ReturnFromBenchmark(!FloatsEqual<T>(determinant, 0) && tmp.HasValue() ? Expected<Matrix<T>>(tmp.Get().GetTranspose() / determinant) : Expected<Matrix<T>>());
        }
        /// @brief Converts matrix to string
        /// @param padding String to pad with
        /// @return String representation of matrix
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override {
            StartBenchmark
            if (height == 1) {
                String ret = CollectionToString(padding) + '[';
                for (size_t x = 0; x < width; x++) ret += MathLib::ToString(At(x, 0)) + (((x + 1) == width) ? "]" : ", ");
                ReturnFromBenchmark(ret);
            }
            else {
                String ret = CollectionToString(padding) + "[\n";
                for (size_t y = 0; y < height; y++) {
                    ret += CollectionToString(padding) + '\t';
                    for (size_t x = 0; x < width; x++) ret += MathLib::ToString(At(x, y)) + (((x + 1) == width && (y + 1) == height) ? "\n" : ", ");
                    if ((y + 1) != height) ret += '\n';
                }
                ReturnFromBenchmark(ret + padding + ']');
            }
        }
        /// @brief Multiplies 2 matrices
        /// @param other Another matrix
        /// @return Result of multiplication
        [[nodiscard]] Expected<Matrix<T>> operator*(const Matrix<T>& other) const {
            StartBenchmark
            if (width != other.height) ReturnFromBenchmark(Expected<Matrix<T>>());
            Matrix<T> ret = Matrix<T>(other.width, height);
            for (size_t y = 0; y < ret.height; y++) {
                for (size_t x = 0; x < ret.width; x++) {
                    ret.At(x, y) = 0;
                    for (size_t x2 = 0; x2 < width; x2++) ret.At(x, y) += At(x2, y) * other.At(x, x2);
                }
            }
            ReturnFromBenchmark(Expected<Matrix<T>>(ret));
        }
        /// @brief Compares current matrix with another matrix
        /// @param other Other matrix
        /// @return Data equality
        [[nodiscard]] bool operator==(const Matrix<T>& other) const {
            StartBenchmark
            bool ret = other.width == width && other.height == height;
            for (size_t y = 0; y < height && ret; y++)
                for (size_t x = 0; x < width && ret; x++) ret = At(x, y) == other.At(x, y);
            ReturnFromBenchmark(ret);
        }
        /// @brief Saves matrix data
        /// @param file File to save matrix data into
        /// @return Status
        [[nodiscard]] virtual bool Save(Writable& file) const override {
            StartBenchmark
            if (!file.Write<size_t>(width) || !file.Write<size_t>(height)) ReturnFromBenchmark(false);
            const size_t size = ptr.GetSize();
            for (size_t i = 0; i < size; i++) {
                const T tmp = ptr.At(i);
                if (!file.Write<T>(tmp)) ReturnFromBenchmark(false);
            }
            ReturnFromBenchmark(true);
        }
        /// @brief Loads matrix data
        /// @param file File to load matrix data from
        /// @return Status
        [[nodiscard]] virtual bool Load(Readable& file) override {
            StartBenchmark
            if (!file.Read<size_t>(width) || !file.Read<size_t>(height)) ReturnFromBenchmark(false);
            ptr = Array<T>(width * height);
            const size_t size = ptr.GetSize();
            for (size_t i = 0; i < size; i++) {
                Expected<T> tmp = file.Read<T>();
                if (!tmp.HasValue()) ReturnFromBenchmark(false);
                ptr.At(i) = tmp.Get();
            }
            ReturnFromBenchmark(true);
        }
        [[nodiscard]] virtual Iterator<const T> begin(void) const override {
            StartAndReturnFromBenchmark(ptr.begin());
        }
        [[nodiscard]] virtual Iterator<const T> end(void) const override {
            StartAndReturnFromBenchmark(ptr.end());
        }
        [[nodiscard]] virtual Iterator<T> begin(void) override {
            StartAndReturnFromBenchmark(ptr.begin());
        }
        [[nodiscard]] virtual Iterator<T> end(void) override {
            StartAndReturnFromBenchmark(ptr.end());
        }

        private:
        /// @brief a + b = [a_0 + b_0, ..., a_n + b_n]
        /// @param other Matrix to add
        void Add(const Matrix<T>& other) {
            StartBenchmark
            if (other.width != width || other.height != height) Panic("Invalid width or height of matrixes for addition");
            for (size_t y = 0; y < height; y++)
                for (size_t x = 0; x < width; x++) At(x, y) += other.At(x, y);
            EndBenchmark
        }
        /// @brief a * s = [a_0 * s, ..., a_n * s]
        /// @param scalar Scalar value to multiply by
        void Multiply(const T& scalar) {
            StartBenchmark
            for (size_t y = 0; y < height; y++)
                for (size_t x = 0; x < width; x++) At(x, y) *= scalar;
            EndBenchmark
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
    [[nodiscard]] Matrix<F> ConvertMatrix(const Matrix<T>& matrix) {
        StartBenchmark
        Matrix<F> ret = Matrix<F>(matrix.GetWidth(), matrix.GetHeight());
        for (size_t y = 0; y < matrix.GetHeight(); y++)
            for (size_t x = 0; x < matrix.GetWidth(); x++) ret.At(x, y) = (F)matrix.At(x, y);
        ReturnFromBenchmark(ret);
    }
    /// @brief Default type for matrices
    using matrix_t = Matrix<num_t>;
}

#endif