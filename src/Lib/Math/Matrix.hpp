#ifndef MathLib_Math_Matrix_H
#define MathLib_Math_Matrix_H
#include "../Interfaces/Printable.hpp"
#include "../Interfaces/Saveable.hpp"
#include "../FunctionT.hpp"
#include "Exponential.hpp"
#include "MathObject.hpp"
#include "Factorial.hpp"

namespace MathLib {
    template <typename T>
    struct Matrix;
    /// @brief Structure representing mathematic matrixes
    /// @tparam T Type of number
    template <typename T>
    struct Matrix : Iteratable<T>, Comparable<Matrix<T>>, Printable, Saveable {
        CreateOperators(Matrix<T>, T)
        CreateExponential(Matrix<T>, IsSquare(), Identity(width))
        /// @brief Creates a new matrix
        /// @param width Width of matrix
        /// @param height Height of matrix
        Matrix(size_t width = 0, size_t height = 0) : width(width), height(height), ptr(width * height) {
            Fill(T());
        }
        /// @brief Creates a new matrix
        /// @param width Width of matrix
        /// @param height Height of matrix
        /// @param func Function representing the matrix
        Matrix(size_t width, size_t height, const Function<T, size_t, size_t>& func) : width(width), height(height), ptr(width * height) {
            for (size_t y = 0; y < height; y++)
                for (size_t x = 0; x < width; x++) AtUnsafe(x, y) = func(x, y);
        }
        /// @brief Creates a new matrix
        /// @param width Width of matrix
        /// @param height Height of matrix
        /// @param arr Values for the matrix
        Matrix(size_t width, size_t height, const Sequence<T>& arr) : width(width), height(height), ptr(CollectionToArray<T>(arr)) {}
        [[nodiscard]] static Matrix<T> Identity(size_t n) {
            Matrix<T> ret = Matrix<T>(n, n);
            for (size_t i = 0; i < n; i++) ret.AtUnsafe(i, i) = 1;
            return ret;
        }
        [[nodiscard]] size_t GetWidth(void) const {
            return width;
        }
        [[nodiscard]] size_t GetHeight(void) const {
            return height;
        }
        [[nodiscard]] Array<T> GetValue(void) const {
            return ptr;
        }
        [[nodiscard]] T* GetPointer(void) {
            return ptr.GetValue();
        }
        [[nodiscard]] const T* GetPointer(void) const {
            return ptr.GetValue();
        }
        [[nodiscard]] Matrix<T> GetRow(size_t y) const {
            if (y >= height) return Matrix<T>();
            Array<T> ret = Array<T>(width);
            for (size_t x = 0; x < width; x++) ret.AtUnsafe(x) = AtUnsafe(x, y);
            return Matrix<T>(width, 1, ret);
        }
        void CopyFromBuffer(const T* buffer) {
            ptr.CopyFromBuffer(buffer);
        }
        [[nodiscard]] bool CopyFromBuffer(const Collection<T>& buffer) {
            return ptr.CopyFromBuffer(buffer);
        }
        [[nodiscard]] bool CopyFromBuffer(const Matrix<T>& buffer) {
            return width == buffer.width && height == buffer.height && ptr.CopyFromBuffer(buffer.ptr);
        }
        [[nodiscard]] bool CopyRow(const Matrix<T>& buffer, size_t y, size_t bufferY) {
            if (width != buffer.width || height <= y || buffer.height <= bufferY) return false;
            const size_t size = width * sizeof(T);
            MemoryCopy((const uint8_t*)buffer.ptr.GetValue() + bufferY * size, (uint8_t*)ptr.GetValue() + y * size, size);
            return true;
        }
        [[nodiscard]] bool IsSquare(void) const {
            return width == height;
        }
        [[nodiscard]] bool IsInside(const Matrix<Interval<T>>& interval) const {
            if (width != interval.GetWidth() || height != interval.GetHeight()) return false;
            for (size_t y = 0; y < height; y++)
                for (size_t x = 0; x < width; x++)
                    if (!interval.AtUnsafe(x, y).Contains(AtUnsafe(x, y))) return false;
            return true;
        }
        [[nodiscard]] bool Swap(const Interval<size_t>& xInterval1, const Interval<size_t>& yInterval1, const Interval<size_t>& xInterval2, const Interval<size_t>& yInterval2) {
            if (xInterval1.GetSize() != xInterval2.GetSize() || yInterval1.GetSize() != yInterval2.GetSize()) return false;
            for (size_t y = 0; y < yInterval1.GetSize(); y++)
                for (size_t x = 0; x < xInterval1.GetSize(); x++) MathLib::Swap<T>(At(x + xInterval1.GetMin(), y + yInterval1.GetMin()), At(x + xInterval2.GetMin(), y + yInterval2.GetMin()));
            return true;
        }
        [[nodiscard]] bool Add(const T& multiple, const Interval<size_t>& xInterval1, const Interval<size_t>& yInterval1, const Interval<size_t>& xInterval2, const Interval<size_t>& yInterval2) {
            if (xInterval1.GetSize() != xInterval2.GetSize() || yInterval1.GetSize() != yInterval2.GetSize()) return false;
            for (size_t y = 0; y < yInterval1.GetSize(); y++)
                for (size_t x = 0; x < xInterval1.GetSize(); x++) At(x + xInterval1.GetMin(), y + yInterval1.GetMin()) += At(x + xInterval2.GetMin(), y + yInterval2.GetMin()) * multiple;
            return true;
        }
        void Multiply(const T& multiple, const Interval<size_t>& xInterval, const Interval<size_t>& yInterval) {
            for (size_t y = yInterval.GetMin(); y < yInterval.GetMax(); y++)
                for (size_t x = xInterval.GetMin(); x < xInterval.GetMax(); x++) At(x, y) *= multiple;
        }
        [[nodiscard]] bool Insert(const Matrix<T>& other, const Interval<size_t>& xInterval1, const Interval<size_t>& yInterval1, const Interval<size_t>& xInterval2, const Interval<size_t>& yInterval2) {
            if (xInterval1.GetSize() != xInterval2.GetSize() || yInterval1.GetSize() != yInterval2.GetSize()) return false;
            for (size_t y = 0; y < yInterval1.GetSize(); y++)
                for (size_t x = 0; x < xInterval1.GetSize(); x++) At(x + xInterval1.GetMin(), y + yInterval1.GetMin()) = other.At(x + xInterval2.GetMin(), y + yInterval2.GetMin());
            return true;
        }
        [[nodiscard]] bool IsFilledWith(const T& value, const Interval<size_t>& xInterval, const Interval<size_t>& yInterval) const {
            for (size_t y = yInterval.GetMin(); y < yInterval.GetMax(); y++)
                for (size_t x = xInterval.GetMin(); x < xInterval.GetMax(); x++)
                    if (!FloatsEqual<T>(At(x, y), value)) return false;
            return true;
        }
        /// @brief Checks whether the matrix is multiple of another matrix
        /// @param other Another matrix
        /// @return Check status
        [[nodiscard]] bool IsMultipleOf(const Matrix<T>& other, const Interval<size_t>& xInterval1, const Interval<size_t>& yInterval1, const Interval<size_t>& xInterval2, const Interval<size_t>& yInterval2) const {
            if (xInterval1.GetSize() != xInterval2.GetSize() || yInterval1.GetSize() != yInterval2.GetSize()) return false;
            T prev = 0;
            for (size_t y = 0; y < yInterval1.GetSize(); y++) {
                for (size_t x = 0; x < xInterval1.GetSize(); x++) {
                    const T v1 = At(x + xInterval1.GetMin(), y + yInterval1.GetMin());
                    const T v2 = other.At(x + xInterval2.GetMin(), y + yInterval2.GetMin());
                    if (FloatsEqual<T>(v1, 0) && FloatsEqual<T>(v2, 0)) continue;
                    const T div = v1 / v2;
                    if (FloatsEqual<T>(prev, 0)) prev = div;
                    if (!FloatsEqual<T>(prev, div)) return false;
                }
            }
            return true;
        }
        [[nodiscard]] bool IsMultipleOf(const Interval<size_t>& xInterval1, const Interval<size_t>& yInterval1, const Interval<size_t>& xInterval2, const Interval<size_t>& yInterval2) const {
            if (xInterval1.GetSize() != xInterval2.GetSize() || yInterval1.GetSize() != yInterval2.GetSize()) return false;
            T prev = 0;
            for (size_t y = 0; y < yInterval1.GetSize(); y++) {
                for (size_t x = 0; x < xInterval1.GetSize(); x++) {
                    const T v1 = At(xInterval1.GetMin() + x, yInterval1.GetMin() + y);
                    const T v2 = At(xInterval2.GetMin() + x, yInterval2.GetMin() + y);
                    if (FloatsEqual<T>(v1, 0) && FloatsEqual<T>(v2, 0)) continue;
                    const T div = v1 / v2;
                    if (FloatsEqual<T>(prev, 0)) prev = div;
                    if (!FloatsEqual<T>(prev, div)) return false;
                }
            }
            return true;
        }
        #define MakeFillBytes(n)                \
        void FillBytes##n(uint##n##_t value) {  \
            ptr.FillBytes##n(value);            \
        }
        MakeFillBytes(8)
        MakeFillBytes(16)
        MakeFillBytes(32)
        MakeFillBytes(64)
        #undef MakeFillBytes
        void Fill(const T& v) {
            for (T& val : ptr) val = v;
        }
        void Random(const T& min, const T& max) {
            for (T& val : ptr) val = RandomNumber(min, max);
        }
        /// @brief Returns data at specified position
        /// @param x X position
        /// @param y Y position
        /// @return Data at specified position
        [[nodiscard]] T& AtUnsafe(size_t x, size_t y) {
            return ptr.AtUnsafe(y * width + x);
        }
        /// @brief Returns data at specified position
        /// @param x X position
        /// @param y Y position
        /// @return Data at specified position
        [[nodiscard]] T AtUnsafe(size_t x, size_t y) const {
            return ptr.AtUnsafe(y * width + x);
        }
        /// @brief Returns data at specified position
        /// @param x X position
        /// @param y Y position
        /// @return Data at specified position
        [[nodiscard]] T& At(size_t x, size_t y) {
            if (x < width && y < height) return AtUnsafe(x, y);
            Panic("Index out of bounds");
        }
        /// @brief Returns data at specified position
        /// @param x X position
        /// @param y Y position
        /// @return Data at specified position
        [[nodiscard]] T At(size_t x, size_t y) const {
            if (x < width && y < height) return AtUnsafe(x, y);
            Panic("Index out of bounds");
        }
        [[nodiscard]] T GetSum(void) const {
            return ptr.Sum(0);
        }
        [[nodiscard]] Expected<Matrix<T>> HadamardProduct(const Matrix<T>& other) const {
            if (width != other.GetWidth() || height != other.GetHeight()) return Expected<Matrix<T>>();
            Matrix<T> ret = *this;
            for (size_t y = 0; y < height; y++)
                for (size_t x = 0; x < width; x++) ret.AtUnsafe(x, y) *= other.AtUnsafe(x, y);
            return ret;
        }
        /// @brief a = (a . b) / (|a| * |b|)
        /// @param other Other matrix
        /// @return Angle between 2 matrices
        [[nodiscard]] T GetAngle(const Matrix<T>& other) const {
            return Dot(other) / (GetLength() * other.GetLength());
        }
        /// @brief |a|^2 = a . a
        /// @return Squared length of the vector
        [[nodiscard]] T GetLengthSquared(void) const {
            return Dot(*this);
        }
        /// @brief |a| = sqrt(a . a)
        /// @return Length of the vector
        [[nodiscard]] T GetLength(void) const {
            return Sqrt(GetLengthSquared());
        }
        /// @brief ^a = a / |a|
        /// @return Normalized matrix
        [[nodiscard]] Matrix<T> Normalize(void) const {
            const T len = GetLength();
            return FloatsEqual<T>(len, 0) ? *this : (*this / len);
        }
        /// @brief a . b = a_0 * b_0 + ... + a_n * b_n
        /// @param other Other matrix
        /// @return Dot product of 2 vectors
        [[nodiscard]] T Dot(const Matrix<T>& other) const {
            if (other.width != width || other.height != height) return nan;
            T ret = 0;
            for (size_t y = 0; y < height; y++)
                for (size_t x = 0; x < width; x++) ret += AtUnsafe(x, y) * other.AtUnsafe(x, y);
            return ret;
        }
        /// @brief ln(A) = (-1)^(1 + 1) * ((A - I)^1 / 1) + ... + (-1)^(1 + ∞) * ((A - I)^∞ / ∞)
        /// @return Logarithm of matrix
        [[nodiscard]] Expected<Matrix<T>> Log(void) const {
            const Matrix<T> identity = Identity(width);
            Matrix<T> ret = Matrix<T>(width, height);
            for (T k = 1; k < 100; k++) {
                const Expected<Matrix<T>> tmp = (*this - identity).UnsignedPow(k);
                if (!tmp.HasValue()) return Expected<Matrix<T>>();
                ret += tmp.Get() * Pow(-1, k + 1) / k;
            }
            return Expected<Matrix<T>>(ret);
        }
        /// @brief e^X = X^0 / 0! + ... + X^∞ / ∞!
        /// @return Exponential of matrix
        [[nodiscard]] Expected<Matrix<T>> Exponential(void) const {
            Matrix<T> ret = Matrix<T>(width, height);
            for (size_t k = 0; k < 100; k++) {
                const Expected<Matrix<T>> tmp = UnsignedPow(k);
                if (!tmp.HasValue()) return Expected<Matrix<T>>();
                ret += tmp.Get() / Factorial<T>(k, 1);
            }
            return Expected<Matrix<T>>(ret);
        }
        /// @brief X^n = exp(ln(X) * n)
        /// @param n Exponent
        /// @return Power of matrix
        [[nodiscard]] Expected<Matrix<T>> Pow(T n) const {
            if (n < 0) {
                const Expected<Matrix<T>> tmp = GetInverse();
                return tmp.HasValue() ? tmp.Get().Pow(-n) : Expected<Matrix<T>>();
            }
            const Expected<Matrix<T>> tmp = Log();
            return tmp.HasValue() ? (tmp.Get() * n).Exponential() : Expected<Matrix<T>>();
        }
        /// @brief X^n = exp(ln(X) * n)
        /// @param n Exponent matrix
        /// @return Power of matrix
        [[nodiscard]] Expected<Matrix<T>> Pow(Matrix<T> n) const {
            const Expected<Matrix<T>> tmp = Log();
            return tmp.HasValue() ? (tmp.Get() * n).Exponential() : Expected<Matrix<T>>();
        }
        /// @brief Returns determinant of the matrix
        /// @return Determinant of the matrix
        [[nodiscard]] T GetDeterminant(void) const {
            if (!IsSquare()) return nan;
            if (!width) return 1;
            if (width == 1) return AtUnsafe(0, 0);
            if (width == 2) return AtUnsafe(0, 0) * AtUnsafe(1, 1) - AtUnsafe(0, 1) * AtUnsafe(1, 0);
            T ret = 0;
            int8_t sign = 1;
            for (size_t i = 0; i < width; i++) {
                Matrix<T> sub = Matrix<T>(width - 1, width - 1);
                for (size_t m = 1; m < width; m++) {
                    size_t z = 0;
                    for (size_t n = 0; n < width; n++)
                        if (n != i) sub.AtUnsafe(m - 1, z++) = AtUnsafe(m, n);
                }
                ret += sign * AtUnsafe(0, i) * sub.GetDeterminant();
                sign *= -1;
            }
            return ret;
        }
        /// @brief Returns transpose of the matrix
        /// @return Transpose of the matrix
        [[nodiscard]] Matrix<T> GetTranspose(void) const {
            Matrix<T> ret = Matrix<T>(height, width);
            for (size_t y = 0; y < height; y++)
                for (size_t x = 0; x < width; x++) ret.AtUnsafe(y, x) = AtUnsafe(x, y);
            return ret;
        }
        /// @brief Returns cofactor of the matrix
        /// @return Cofactor of the matrix
        [[nodiscard]] Expected<Matrix<T>> GetCofactor(void) const {
            if (!IsSquare()) return Expected<Matrix<T>>();
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
                            sub.AtUnsafe(p, q) = AtUnsafe(x, y);
                            q++;
                        }
                        p++;
                    }
                    ret.AtUnsafe(i, j) = MathLib::Pow(-1, i + j) * sub.GetDeterminant();
                }
            }
            return Expected<Matrix<T>>(ret);
        }
        /// @brief Returns inverse of the matrix
        /// @return Inverse of the matrix
        [[nodiscard]] Expected<Matrix<T>> GetInverse(void) const {
            const Expected<Matrix<T>> tmp = GetCofactor();
            const T determinant = GetDeterminant();
            return !FloatsEqual<T>(determinant, 0) && tmp.HasValue() ? Expected<Matrix<T>>(tmp.Get().GetTranspose() / determinant) : Expected<Matrix<T>>();
        }
        /// @brief Converts matrix to string
        /// @param padding String to pad with
        /// @return String representation of matrix
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override {
            if (height == 1) {
                String ret = CollectionToString(padding) + '[';
                for (size_t x = 0; x < width; x++) ret += Formatter<T>::ToString(AtUnsafe(x, 0)) + (((x + 1) == width) ? "]" : ", ");
                return ret;
            }
            String ret = CollectionToString(padding) + "[\n";
            for (size_t y = 0; y < height; y++) {
                ret += CollectionToString(padding) + '\t';
                for (size_t x = 0; x < width; x++) ret += Formatter<T>::ToString(AtUnsafe(x, y)) + (((x + 1) == width && (y + 1) == height) ? "\n" : ", ");
                if ((y + 1) != height) ret += '\n';
            }
            return ret + padding + ']';
        }
        [[nodiscard]] Expected<Matrix<T>> MultiplyAddTransform(const Matrix<T>& multiplier, const Matrix<T>& adder, const Function<T, T>& transform) const {
            if (width != multiplier.height || multiplier.width != adder.width || height != adder.height) return Expected<Matrix<T>>();
            Matrix<T> ret = Matrix<T>(multiplier.width, height);
            for (size_t y = 0; y < ret.height; y++) {
                for (size_t x = 0; x < ret.width; x++) {
                    ret.AtUnsafe(x, y) = adder.AtUnsafe(x, y);
                    for (size_t x2 = 0; x2 < width; x2++) ret.AtUnsafe(x, y) += AtUnsafe(x2, y) * multiplier.AtUnsafe(x, x2);
                    ret.AtUnsafe(x, y) = transform(ret.AtUnsafe(x, y));
                }
            }
            return ret;
        }
        /// @brief Multiplies 2 matrices
        /// @param other Another matrix
        /// @return Result of multiplication
        [[nodiscard]] Expected<Matrix<T>> operator*(const Matrix<T>& other) const {
            if (width != other.height) return Expected<Matrix<T>>();
            Matrix<T> ret = Matrix<T>(other.width, height);
            for (size_t y = 0; y < ret.height; y++)
                for (size_t x = 0; x < ret.width; x++)
                    for (size_t x2 = 0; x2 < width; x2++) ret.AtUnsafe(x, y) += AtUnsafe(x2, y) * other.AtUnsafe(x, x2);
            return ret;
        }
        /// @brief Saves matrix data
        /// @param file File to save matrix data into
        /// @return Status
        [[nodiscard]] virtual bool Save(Writable& file) const override {
            if (!file.Write<size_t>(width) || !file.Write<size_t>(height)) return false;
            const size_t size = ptr.GetSize();
            for (size_t i = 0; i < size; i++)
                if (!file.Write<T>(ptr.AtUnsafe(i))) return false;
            return true;
        }
        /// @brief Loads matrix data
        /// @param file File to load matrix data from
        /// @return Status
        [[nodiscard]] virtual bool Load(Readable& file) override {
            if (!file.Read<size_t>(width) || !file.Read<size_t>(height)) return false;
            ptr = Array<T>(width * height);
            const size_t size = ptr.GetSize();
            for (size_t i = 0; i < size; i++) {
                const Expected<T> tmp = file.Read<T>();
                if (!tmp.HasValue()) return false;
                ptr.AtUnsafe(i) = tmp.Get();
            }
            return true;
        }
        [[nodiscard]] virtual Iterator<const T> begin(void) const override {
            return ptr.begin();
        }
        [[nodiscard]] virtual Iterator<const T> end(void) const override {
            return ptr.end();
        }
        [[nodiscard]] virtual Iterator<T> begin(void) override {
            return ptr.begin();
        }
        [[nodiscard]] virtual Iterator<T> end(void) override {
            return ptr.end();
        }

        protected:
        /// @brief Compares current matrix with another matrix
        /// @param other Other matrix
        /// @return Data equality
        [[nodiscard]] virtual bool Equals(const Matrix<T>& other) const override {
            if (width != other.width || height != other.height) return false;
            for (size_t y = 0; y < height; y++)
                for (size_t x = 0; x < width; x++)
                    if (AtUnsafe(x, y) != other.AtUnsafe(x, y)) return false;
            return true;
        }

        private:
        /// @brief a + b = [a_0 + b_0, ..., a_n + b_n]
        /// @param other Matrix to add
        void Add(const Matrix<T>& other) {
            if (other.width != width || other.height != height) Panic("Invalid width or height of matrixes for addition");
            for (size_t y = 0; y < height; y++)
                for (size_t x = 0; x < width; x++) AtUnsafe(x, y) += other.AtUnsafe(x, y);
        }
        /// @brief a * s = [a_0 * s, ..., a_n * s]
        /// @param scalar Scalar value to multiply by
        void Multiply(const T& scalar) {
            for (size_t y = 0; y < height; y++)
                for (size_t x = 0; x < width; x++) AtUnsafe(x, y) *= scalar;
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
        Matrix<F> ret = Matrix<F>(matrix.GetWidth(), matrix.GetHeight());
        for (size_t y = 0; y < matrix.GetHeight(); y++)
            for (size_t x = 0; x < matrix.GetWidth(); x++) ret.AtUnsafe(x, y) = (F)matrix.AtUnsafe(x, y);
        return ret;
    }
    /// @brief Default type for matrices
    using matrix_t = Matrix<num_t>;
}

#endif