#ifndef MathLib_Math_Complex_H
#define MathLib_Math_Complex_H
#include "../Interfaces/Printable.hpp"
#include "MathObject.hpp"

namespace MathLib {
    String ToString(num_t x);
    num_t MakeNaN(void);
    num_t Pow(num_t x, num_t y);
    num_t InversedTan2(num_t y, num_t x);
    template <typename T>
    [[nodiscard]] String CoefficientToString(const T& x, const String& symbol) {
        StartBenchmark
        if (FloatsEqual<T>(x, 0)) ReturnFromBenchmark("")
        else if (FloatsEqual<T>(x, 1)) ReturnFromBenchmark(symbol)
        else if (FloatsEqual<T>(x, -1)) ReturnFromBenchmark(String('-') + symbol)
        else ReturnFromBenchmark(MathLib::ToString(x) + symbol)
    }
    template <typename T>
    struct Complex : Printable {
        CreateOperators(Complex<T>, T)
        Complex<T>(const T& re = 0, const T& im = 0) : real(re), imaginary(im) {
            EmptyBenchmark
        }
        [[nodiscard]] T GetReal(void) const {
            StartBenchmark
            ReturnFromBenchmark(real);
        }
        [[nodiscard]] T GetImaginary(void) const {
            StartBenchmark
            ReturnFromBenchmark(imaginary);
        }
        [[nodiscard]] T GetLengthSquared(void) const {
            StartBenchmark
            ReturnFromBenchmark(real * real + imaginary * imaginary);
        }
        [[nodiscard]] T GetLength(void) const {
            StartBenchmark
            ReturnFromBenchmark(Sqrt(GetLengthSquared()));
        }
        [[nodiscard]] Complex<T> GetInverse(void) const {
            StartBenchmark
            const T tmp = GetLengthSquared();
            ReturnFromBenchmark(Complex<T>(real / tmp, -imaginary / tmp));
        }
        [[nodiscard]] T GetArgument(void) const {
            StartBenchmark
            ReturnFromBenchmark(InversedTan2(imaginary, real));
        }
        [[nodiscard]] T ToReal(void) const {
            StartBenchmark
            ReturnFromBenchmark(FloatsEqual<T>(imaginary, 0) ? real : MakeNaN());
        }
        /// @brief Converts struct to string
        /// @param padding String to pad with
        /// @return String representation
        [[nodiscard]] virtual String ToString(const String& padding = "") const override {
            StartBenchmark
            if (FloatsEqual<T>(real, 0)) ReturnFromBenchmark(padding + (FloatsEqual<T>(imaginary, 0) ? "0" : CoefficientToString(imaginary, "i")))
            else if (FloatsEqual<T>(imaginary, 0)) ReturnFromBenchmark(padding + MathLib::ToString(real))
            else ReturnFromBenchmark(padding + MathLib::ToString(real) + " + " + MathLib::ToString(imaginary) + 'i')
        }
        [[nodiscard]] Complex<T> operator/(const Complex<T>& other) const {
            StartBenchmark
            ReturnFromBenchmark(*this * other.GetInverse());
        }
        [[nodiscard]] Complex<T> operator*(const Complex<T>& other) const {
            StartBenchmark
            ReturnFromBenchmark(Complex<T>(real * other.real - imaginary * other.imaginary, real * other.imaginary + imaginary * other.real));
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
            StartBenchmark
            ReturnFromBenchmark(FloatsEqual<T>(real, other.real) && FloatsEqual<T>(imaginary, other.imaginary));
        }
        [[nodiscard]] explicit operator num_t(void) const {
            StartBenchmark
            ReturnFromBenchmark(ToReal());
        }
        [[nodiscard]] explicit operator size_t(void) const {
            StartBenchmark
            ReturnFromBenchmark((size_t)ToReal());
        }
        [[nodiscard]] explicit operator ssize_t(void) const {
            StartBenchmark
            ReturnFromBenchmark((ssize_t)ToReal());
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
        StartBenchmark
        ReturnFromBenchmark(complex.GetInverse() * scalar);
    }
    template <typename T>
    [[nodiscard]] bool operator<(const T& scalar, const Complex<T>& complex) {
        StartBenchmark
        ReturnFromBenchmark(FloatsEqual<T>(complex.GetImaginary(), 0) && scalar < complex.GetReal())
    }
    /// @brief Default type for complex numbers
    using complex_t = Complex<num_t>;
}

#endif