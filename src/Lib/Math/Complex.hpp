#ifndef MathLib_Math_Complex_H
#define MathLib_Math_Complex_H
#include "../Interfaces/Printable.hpp"
#include "MathObject.hpp"
#include "Orderable.hpp"

namespace MathLib {
    extern const num_t nan;
    String ToString(num_t x);
    num_t Pow(num_t x, num_t y);
    num_t InversedTan2(num_t y, num_t x);
    template <typename T>
    [[nodiscard]] String CoefficientToString(const T& x, const Sequence<char>& symbol) {
        StartBenchmark
        if (FloatsEqual<T>(x, 0)) ReturnFromBenchmark("")
        else if (FloatsEqual<T>(x, 1)) ReturnFromBenchmark(CollectionToString(symbol))
        else if (FloatsEqual<T>(x, -1)) ReturnFromBenchmark('-'_M + symbol)
        else ReturnFromBenchmark(MathLib::ToString(x) + symbol)
    }
    template <typename T>
    struct Complex : Orderable, Printable {
        CreateOperators(Complex<T>, T)
        Complex<T>(const T& re = 0, const T& im = 0) : real(re), imaginary(im) {
            EmptyBenchmark
        }
        [[nodiscard]] T GetReal(void) const {
            StartAndReturnFromBenchmark(real);
        }
        [[nodiscard]] T GetImaginary(void) const {
            StartAndReturnFromBenchmark(imaginary);
        }
        [[nodiscard]] T GetLengthSquared(void) const {
            StartAndReturnFromBenchmark(real * real + imaginary * imaginary);
        }
        [[nodiscard]] T GetLength(void) const {
            StartAndReturnFromBenchmark(Sqrt(GetLengthSquared()));
        }
        [[nodiscard]] Complex<T> GetInverse(void) const {
            StartBenchmark
            const T tmp = GetLengthSquared();
            ReturnFromBenchmark(Complex<T>(real / tmp, -imaginary / tmp));
        }
        [[nodiscard]] T GetArgument(void) const {
            StartAndReturnFromBenchmark(InversedTan2(imaginary, real));
        }
        [[nodiscard]] T ToReal(void) const {
            StartAndReturnFromBenchmark(FloatsEqual<T>(imaginary, 0) ? real : nan);
        }
        /// @brief Converts struct to string
        /// @param padding String to pad with
        /// @return String representation
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override {
            StartBenchmark
            if (FloatsEqual<T>(real, 0)) ReturnFromBenchmark(CollectionToString(padding) + (FloatsEqual<T>(imaginary, 0) ? "0" : CoefficientToString(imaginary, 'i'_M)))
            else if (FloatsEqual<T>(imaginary, 0)) ReturnFromBenchmark(CollectionToString(padding) + MathLib::ToString(real))
            else ReturnFromBenchmark(CollectionToString(padding) + MathLib::ToString(real) + " + " + MathLib::ToString(imaginary) + 'i')
        }
        [[nodiscard]] Complex<T> operator/(const Complex<T>& other) const {
            StartAndReturnFromBenchmark(*this * other.GetInverse());
        }
        [[nodiscard]] Complex<T> operator*(const Complex<T>& other) const {
            StartAndReturnFromBenchmark(Complex<T>(real * other.real - imaginary * other.imaginary, real * other.imaginary + imaginary * other.real));
        }
        Complex<T>& operator/=(const Complex<T>& other) {
            StartBenchmark
            const Complex<T> tmp = *this / other;
            real = tmp.real;
            imaginary = tmp.imaginary;
            ReturnFromBenchmark(*this);
        }
        Complex<T>& operator*=(const Complex<T>& other) {
            StartBenchmark
            const Complex<T> tmp = *this * other;
            real = tmp.real;
            imaginary = tmp.imaginary;
            ReturnFromBenchmark(*this);
        }
        [[nodiscard]] bool operator==(const Complex<T>& other) const {
            StartAndReturnFromBenchmark(FloatsEqual<T>(real, other.real) && FloatsEqual<T>(imaginary, other.imaginary));
        }
        [[nodiscard]] explicit operator num_t(void) const {
            StartAndReturnFromBenchmark(ToReal());
        }
        [[nodiscard]] explicit operator size_t(void) const {
            StartAndReturnFromBenchmark((size_t)ToReal());
        }
        [[nodiscard]] explicit operator ssize_t(void) const {
            StartAndReturnFromBenchmark((ssize_t)ToReal());
        }

        protected:
        [[nodiscard]] virtual bool LessThanEqual(const Orderable& other_) const override {
            const Complex<T>& other = (const Complex<T>&)other_;
            return FloatsEqual<T>(imaginary, other.GetImaginary()) && real <= other.GetReal();
        }

        private:
        T real;
        T imaginary;

        /// @brief a + b = re(a) + re(b) + (im(a) + im(b))i
        /// @param other Complex number to add
        void Add(const Complex<T>& other) {
            StartBenchmark
            real += other.real;
            imaginary += other.imaginary;
            EndBenchmark
        }
        /// @brief a * s = (re(a) + im(a)i) * s
        /// @param scalar Scalar value to multiply by
        void Multiply(const T& scalar) {
            StartBenchmark
            real *= scalar;
            imaginary *= scalar;
            EndBenchmark
        }
    };
    template <typename T>
    [[nodiscard]] Complex<T> operator/(const T& scalar, const Complex<T>& complex) {
        StartAndReturnFromBenchmark(complex.GetInverse() * scalar);
    }
    template <typename T>
    [[nodiscard]] bool operator<(const T& scalar, const Complex<T>& complex) {
        StartAndReturnFromBenchmark(Complex<T>(scalar, 0) < complex);
    }
    /// @brief Default type for complex numbers
    using complex_t = Complex<num_t>;
}

#endif