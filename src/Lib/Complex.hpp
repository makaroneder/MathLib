#ifndef Complex_H
#define Complex_H
#include "Interfaces/Printable.hpp"
#include "MathObject.hpp"

String ToString(num_t x);
num_t Pow(num_t x, num_t y);
template <typename T>
struct Complex : Printable {
    CreateOperators(Complex<T>, T)
    Complex<T>(const T& re = 0, const T& im = 0) : real(re), imaginary(im) {}
    T GetReal(void) const {
        return real;
    }
    T GetImaginary(void) const {
        return imaginary;
    }
    T GetLengthSquared(void) const {
        return Pow(real, 2) + Pow(imaginary, 2);
    }
    T GetLength(void) const {
        return Sqrt(GetLengthSquared());
    }
    Complex<T> GetInverse(void) const {
        const T tmp = GetLengthSquared();
        return Complex<T>(real / tmp, -imaginary / tmp);
    }
    T GetArgument(void) const {
        return InversedTan2(imaginary, real);
    }
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    virtual String ToString(const String& padding = "") const override {
        return padding + ::ToString(real) + " + " + ::ToString(imaginary) + 'i';
    }
    Complex<T> operator/(const Complex<T>& other) const {
        return *this * other.GetInverse();
    }
    Complex<T> operator*(const Complex<T>& other) const {
        return Complex<T>(real * other.real - imaginary * other.imaginary, real * other.imaginary + imaginary * other.real);
    }
    Complex<T> operator/=(const Complex<T>& other) {
        const Complex<T> tmp = *this / other;
        real = tmp.real;
        imaginary = tmp.imaginary;
        return tmp;
    }
    Complex<T> operator*=(const Complex<T>& other) {
        const Complex<T> tmp = *this * other;
        real = tmp.real;
        imaginary = tmp.imaginary;
        return tmp;
    }

    private:
    T real;
    T imaginary;

    /// @brief a + b = re(a) + re(b) + (im(a) + im(b))i
    /// @param other Complex number to add
    void Add(const Complex<T>& other) {
        real += other.real;
        imaginary += other.imaginary;
    }
    /// @brief a * s = (re(a) + im(a)i) * s
    /// @param scalar Scalar value to multiply by
    void Multiply(const T& scalar) {
        real *= scalar;
        imaginary *= scalar;
    }
};
template <typename T>
Complex<T> operator/(const T& scalar, const Complex<T>& complex) {
    return complex.GetInverse() * scalar;
}
/// @brief Default type for complex numbers
typedef Complex<num_t> complex_t;

#endif