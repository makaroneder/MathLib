#ifndef MathLib_Math_Rational_H
#define MathLib_Math_Rational_H
#include "../Interfaces/Printable.hpp"
#include "MathObject.hpp"

namespace MathLib {
    struct Rational {
        Rational(bool sign, size_t a, size_t b);
        Rational(ssize_t a, size_t b);
        num_t ToFloat(void) const;
        Rational Simplify(void) const;
        /// @brief (a / b) * (c / d) = ac / bd
        /// @param other Other rational number
        /// @return Product of 2 rational numbers
        Rational operator*(const Rational& other) const;
        /// @brief (a / b) / (c / d) = (a / b) * (d / c) = ad / bc
        /// @param other Other rational number
        /// @return Quotient of 2 rational numbers
        Rational operator/(const Rational& other) const;

        private:
        bool sign;
        size_t a;
        size_t b;
    };
}

#endif