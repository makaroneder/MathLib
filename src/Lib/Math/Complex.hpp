#ifndef MathLib_Math_Complex_H
#define MathLib_Math_Complex_H
#include "../Interfaces/Printable.hpp"
#include "../Interfaces/Orderable.hpp"
#include "MathObject.hpp"

namespace MathLib {
    extern const num_t nan;
    String ToString(num_t x);
    num_t Pow(num_t x, num_t y);
    num_t InversedTan2(num_t y, num_t x);
    template <typename T>
    [[nodiscard]] String CoefficientToString(const T& x, const Sequence<char>& symbol) {
        if (FloatsEqual<T>(x, 0)) return "";
        if (FloatsEqual<T>(x, 1)) return CollectionToString(symbol);
        if (FloatsEqual<T>(x, -1)) return '-'_M + symbol;
        return Formatter<T>::ToString(x) + symbol;
    }
    template <typename T>
    struct Complex;
    template <typename T>
    struct Complex : Orderable<Complex<T>>, Printable {
        CreateOperators(Complex<T>, T)
        Complex<T>(const T& re = 0, const T& im = 0) : real(re), imaginary(im) {}
        [[nodiscard]] T GetReal(void) const {
            return real;
        }
        [[nodiscard]] T GetImaginary(void) const {
            return imaginary;
        }
        [[nodiscard]] T GetLengthSquared(void) const {
            return real * real + imaginary * imaginary;
        }
        [[nodiscard]] T GetLength(void) const {
            return Sqrt(GetLengthSquared());
        }
        [[nodiscard]] Complex<T> GetInverse(void) const {
            const T tmp = GetLengthSquared();
            return Complex<T>(real / tmp, -imaginary / tmp);
        }
        [[nodiscard]] T GetArgument(void) const {
            return InversedTan2(imaginary, real);
        }
        [[nodiscard]] T ToReal(void) const {
            return FloatsEqual<T>(imaginary, 0) ? real : nan;
        }
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override {
            if (FloatsEqual<T>(real, 0)) return CollectionToString(padding) + (FloatsEqual<T>(imaginary, 0) ? "0" : CoefficientToString(imaginary, 'i'_M));
            if (FloatsEqual<T>(imaginary, 0)) return CollectionToString(padding) + Formatter<T>::ToString(real);
            return CollectionToString(padding) + Formatter<T>::ToString(real) + " + " + Formatter<T>::ToString(imaginary) + 'i';
        }
        [[nodiscard]] Complex<T> operator/(const Complex<T>& other) const {
            return *this * other.GetInverse();
        }
        [[nodiscard]] Complex<T> operator*(const Complex<T>& other) const {
            return Complex<T>(real * other.real - imaginary * other.imaginary, real * other.imaginary + imaginary * other.real);
        }
        Complex<T>& operator/=(const Complex<T>& other) {
            const Complex<T> tmp = *this / other;
            real = tmp.real;
            imaginary = tmp.imaginary;
            return *this;
        }
        Complex<T>& operator*=(const Complex<T>& other) {
            const Complex<T> tmp = *this * other;
            real = tmp.real;
            imaginary = tmp.imaginary;
            return *this;
        }
        [[nodiscard]] bool operator==(const Complex<T>& other) const {
            return FloatsEqual<T>(real, other.real) && FloatsEqual<T>(imaginary, other.imaginary);
        }
        [[nodiscard]] explicit operator num_t(void) const {
            return ToReal();
        }
        [[nodiscard]] explicit operator size_t(void) const {
            return (size_t)ToReal();
        }
        [[nodiscard]] explicit operator ssize_t(void) const {
            return (ssize_t)ToReal();
        }
        [[nodiscard]] virtual bool LessThanEqual(const Complex<T>& other) const override {
            return FloatsEqual<T>(imaginary, other.GetImaginary()) && real <= other.GetReal();
        }

        private:
        T real;
        T imaginary;

        void Add(const Complex<T>& other) {
            real += other.real;
            imaginary += other.imaginary;
        }
        void Multiply(const T& scalar) {
            real *= scalar;
            imaginary *= scalar;
        }
    };
    template <typename T>
    [[nodiscard]] Complex<T> operator/(const T& scalar, const Complex<T>& complex) {
        return complex.GetInverse() * scalar;
    }
    template <typename T>
    [[nodiscard]] bool operator<(const T& scalar, const Complex<T>& complex) {
        return Complex<T>(scalar, 0) < complex;
    }
    using complex_t = Complex<num_t>;
}

#endif