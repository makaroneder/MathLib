#ifndef MathLib_Math_Trigonometry_H
#define MathLib_Math_Trigonometry_H
#include "Constants.hpp"
#include "Host.hpp"

namespace MathLib {
    /// @brief sinc(x) = sin(x) / x
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Sinc of x
    template <typename T>
    [[nodiscard]] T Sinc(const T& x) {
        return FloatsEqual<T>(x, 0) ? 1 : Sin(x) / x;
    }
    /// @brief cos(x) = sin(pi / 2 - x)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Cosine of x
    template <typename T>
    [[nodiscard]] T Cos(const T& x) {
        return Sin(T(pi / 2) - x);
    }
    /// @brief tan(x) = sin(x) / cos(x)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Tangent of x
    template <typename T>
    [[nodiscard]] T Tan(const T& x) {
        return Sin(x) / Cos<T>(x);
    }
    /// @brief cot(x) = cos(x) / sin(x)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Cotangent of x
    template <typename T>
    [[nodiscard]] constexpr T Cot(const T& x) {
        return Cos<T>(x) / Sin(x);
    }
    /// @brief sec(x) = 1 / cos(x)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Secant of x
    template <typename T>
    [[nodiscard]] constexpr T Sec(const T& x) {
        return T(1) / Cos<T>(x);
    }
    /// @brief csc(x) = 1 / sin(x)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Cosecant of x
    template <typename T>
    [[nodiscard]] constexpr T Csc(const T& x) {
        return T(1) / Sin(x);
    }
    /// @brief crd(x) = 2sin(x / 2)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Chord of x
    template <typename T>
    [[nodiscard]] constexpr T Crd(const T& x) {
        return Sin(x / 2) * 2;
    }
    /// @brief versin(x) = 1 - cos(x)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Versed sine of x
    template <typename T>
    [[nodiscard]] constexpr T Versin(const T& x) {
        return T(1) - Cos<T>(x);
    }
    /// @brief coversin(x) = 1 - sin(x)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Coversed sine of x
    template <typename T>
    [[nodiscard]] constexpr T Coversin(const T& x) {
        return T(1) - Sin(x);
    }
    /// @brief vercos(x) = 1 + cos(x)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Versed cosine of x
    template <typename T>
    [[nodiscard]] constexpr T Vercos(const T& x) {
        return T(1) + Cos<T>(x);
    }
    /// @brief covercos(x) = 1 + sin(x)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Coversed cosine of x
    template <typename T>
    [[nodiscard]] constexpr T Covercos(const T& x) {
        return T(1) + Sin(x);
    }
    /// @brief haversin(x) = (1 - cos(x)) / 2
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Haversed sine of x
    template <typename T>
    [[nodiscard]] constexpr T Haversin(const T& x) {
        return (T(1) - Cos<T>(x)) / 2;
    }
    /// @brief hacoversin(x) = (1 - sin(x)) / 2
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Hacoversed sine of x
    template <typename T>
    [[nodiscard]] constexpr T Hacoversin(const T& x) {
        return (T(1) - Sin(x)) / 2;
    }
    /// @brief havercos(x) = (1 + cos(x)) / 2
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Haversed cosine of x
    template <typename T>
    [[nodiscard]] constexpr T Havercos(const T& x) {
        return (T(1) + Cos<T>(x)) / 2;
    }
    /// @brief hacovercos(x) = (1 + sin(x)) / 2
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Hacoversed cosine of x
    template <typename T>
    [[nodiscard]] constexpr T Hacovercos(const T& x) {
        return (T(1) + Sin(x)) / 2;
    }
    /// @brief exsec(x) = versin(x) / cos(x)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Exsecant of x
    template <typename T>
    [[nodiscard]] constexpr T Exsec(const T& x) {
        return Versin<T>(x) / Cos<T>(x);
    }
    /// @brief excsc(x) = coversin(x) / sin(x)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Excosecant of x
    template <typename T>
    [[nodiscard]] constexpr T Excsc(const T& x) {
        return Coversin<T>(x) / Sin(x);
    }
    /// @brief sinh(x) = (exp(x) - exp(-x)) / 2
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Hyperbolic sine of x
    template <typename T>
    [[nodiscard]] T HyperbolicSin(const T& x) {
        return (Exp(x) - Exp(-x)) / 2;
    }
    /// @brief cosh(x) = (exp(x) + exp(-x)) / 2
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Hyperbolic cosine of x
    template <typename T>
    [[nodiscard]] T HyperbolicCos(const T& x) {
        return (Exp(x) + Exp(-x)) / 2;
    }
    /// @brief tanh(x) = sinh(x) / cosh(x)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Hyperbolic tangent of x
    template <typename T>
    [[nodiscard]] T HyperbolicTan(const T& x) {
        return HyperbolicSin<T>(x) / HyperbolicCos<T>(x);
    }
    /// @brief coth(x) = (exp(2x) + 1) / (exp(2x) - 1)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Hyperbolic cotangent of x
    template <typename T>
    [[nodiscard]] constexpr T HyperbolicCot(const T& x) {
        return (Exp(x * 2) + 1) / (Exp(x * 2) - 1);
    }
    /// @brief sech(x) = 2e^x / (e^2x + 1)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Hyperbolic secant of x
    template <typename T>
    [[nodiscard]] constexpr T HyperbolicSec(const T& x) {
        return (Exp(x) * 2) / (Exp(x * 2) + 1);
    }
    /// @brief csch(x) = 2e^x / (e^2x - 1)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Hyperbolic cosecant of x
    template <typename T>
    [[nodiscard]] constexpr T HyperbolicCsc(const T& x) {
        return (Exp(x) * 2) / (Exp(x * 2) - 1);
    }
    /// @brief arccos(x) = pi / 2 - arcsin(x)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Inversed sine of x
    template <typename T>
    [[nodiscard]] constexpr T InversedCos(const T& x) {
        return T(pi / 2) - InversedSin(x);
    }
    /// @brief arctan(x) = arcsin(x / sqrt(1 + x^2))
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Inversed cotangent of x
    template <typename T>
    [[nodiscard]] constexpr T InversedTan(const T& x) {
        return InversedSin(x / Sqrt(x * x + 1));
    }
    /// @brief arccot(x) = pi / 2 - arctan(x)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Inversed cotangent of x
    template <typename T>
    [[nodiscard]] constexpr T InversedCot(const T& x) {
        return T(pi / 2) - InversedTan<T>(x);
    }
    /// @brief arcsec(x) = arccos(1 / x)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Inversed secant of x
    template <typename T>
    [[nodiscard]] constexpr T InversedSec(const T& x) {
        return InversedCos<T>(T(1) / x);
    }
    /// @brief arccsc(x) = arcsin(1 / x)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Inversed cosecant of x
    template <typename T>
    [[nodiscard]] constexpr T InversedCsc(const T& x) {
        return InversedSin(T(1) / x);
    }
    /// @brief arccrd(x) = 2arcsin(x / 2)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Inversed chord of x
    template <typename T>
    [[nodiscard]] constexpr T InversedCrd(const T& x) {
        return InversedSin(x / 2) * 2;
    }
    /// @brief arcexsec(x) = arcsec(x + 1)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Inversed exsecant of x
    template <typename T>
    [[nodiscard]] constexpr T InversedExsec(const T& x) {
        return InversedSec<T>(x + 1);
    }
    /// @brief arcexcsc(x) = arccsc(x + 1)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Inversed excosecant of x
    template <typename T>
    [[nodiscard]] constexpr T InversedExcsc(const T& x) {
        return InversedCsc<T>(x + 1);
    }
    /// @brief arcversin(x) = arccos(1 - x)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Inversed versed sine of x
    template <typename T>
    [[nodiscard]] constexpr T InversedVersin(const T& x) {
        return InversedCos<T>(T(1) - x);
    }
    /// @brief arccoversin(x) = arcsin(1 - x)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Inversed coversed sine of x
    template <typename T>
    [[nodiscard]] constexpr T InversedCoversin(const T& x) {
        return InversedSin(T(1) - x);
    }
    /// @brief arcvercos(x) = arccos(x - 1)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Inversed versed cosine of x
    template <typename T>
    [[nodiscard]] constexpr T InversedVercos(const T& x) {
        return InversedCos<T>(x - 1);
    }
    /// @brief arccovercos(x) = arcsin(x - 1)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Inversed coversed cosine of x
    template <typename T>
    [[nodiscard]] constexpr T InversedCovercos(const T& x) {
        return InversedSin(x - 1);
    }
    /// @brief archaversin(x) = arccos(1 - 2x)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Inversed haversed sine of x
    template <typename T>
    [[nodiscard]] constexpr T InversedHaversin(const T& x) {
        return InversedCos<T>(T(1) - x * 2);
    }
    /// @brief archacoversin(x) = arcsin(1 - 2x)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Inversed hacoversed sine of x
    template <typename T>
    [[nodiscard]] constexpr T InversedHacoversin(const T& x) {
        return InversedSin(T(1) - x * 2);
    }
    /// @brief archavercos(x) = arccos(2x - 1)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Inversed haversed cosine of x
    template <typename T>
    [[nodiscard]] constexpr T InversedHavercos(const T& x) {
        return InversedCos<T>(x * 2 - 1);
    }
    /// @brief archacovercos(x) = arcsin(2x - 1)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Inversed hacoversed cosine of x
    template <typename T>
    [[nodiscard]] constexpr T InversedHacovercos(const T& x) {
        return InversedSin(x * 2 - 1);
    }
    /// @brief arcsinh(x) = ln(x + sqrt(x^2 + 1))
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Inversed hyperbolic sine of x
    template <typename T>
    [[nodiscard]] T InversedHyperbolicSin(const T& x) {
        return NaturalLog(x + Sqrt(x * x + 1));
    }
    /// @brief arccosh(x) = ln(x + sqrt(x^2 - 1))
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Inversed hyperbolic cosine of x
    template <typename T>
    [[nodiscard]] T InversedHyperbolicCos(const T& x) {
        return NaturalLog(x + Sqrt(x * x - 1));
    }
    /// @brief arctanh(x) = ln((1 + x) / (1 - x)) / 2
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Inversed hyperbolic tangent of x
    template <typename T>
    [[nodiscard]] T InversedHyperbolicTan(const T& x) {
        return NaturalLog((x + 1) / (T(1) - x)) / 2;
    }
    /// @brief arccoth(x) = ln((x + 1) / (x - 1)) / 2
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Inversed hyperbolic cotangent of x
    template <typename T>
    [[nodiscard]] constexpr T InversedHyperbolicCot(const T& x) {
        return NaturalLog((x + 1) / (x - 1)) / 2;
    }
    /// @brief arcsech(x) = ln((1 / x) + sqrt(2 $ (1 / x^2 - 1)))
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Inversed hyperbolic secant of x
    template <typename T>
    [[nodiscard]] constexpr T InversedHyperbolicSec(const T& x) {
        return InversedHyperbolicCos<T>(T(1) / x);
    }
    /// @brief arccsch(x) = ln((1 / x) + sqrt(2 $ (1 / x^2 + 1)))
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Inversed hyperbolic cosecant of x
    template <typename T>
    [[nodiscard]] constexpr T InversedHyperbolicCsc(const T& x) {
        return NaturalLog(T(1) / x + Sqrt(T(1) / (x * x) + 1));
    }
}

#endif