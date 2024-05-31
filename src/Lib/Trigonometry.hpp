#ifndef Trigonometry_H
#define Trigonometry_H
#include "Constants.hpp"

/// @brief cos(x) = sin(pi / 2 - x)
/// @tparam T Type of number
/// @param x Radians
/// @return Cosine of x
template <typename T>
T Cos(T x) {
    const T tmp = pi / 2;
    return Sin(tmp - x);
}
/// @brief tan(x) = sin(x) / cos(x)
/// @tparam T Type of number
/// @param x Radians
/// @return Tangent of x
template <typename T>
T Tan(T x) {
    return Sin(x) / Cos<T>(x);
}
/// @brief cot(x) = cos(x) / sin(x)
/// @tparam T Type of number
/// @param x Radians
/// @return Cotangent of x
template <typename T>
constexpr T Cot(T x) {
    return Cos<T>(x) / Sin(x);
}
/// @brief sec(x) = 1 / cos(x)
/// @tparam T Type of number
/// @param x Radians
/// @return Secant of x
template <typename T>
constexpr T Sec(T x) {
    const T one = 1;
    return one / Cos<T>(x);
}
/// @brief csc(x) = 1 / sin(x)
/// @tparam T Type of number
/// @param x Radians
/// @return Cosecant of x
template <typename T>
constexpr T Csc(T x) {
    const T one = 1;
    return one / Sin(x);
}
/// @brief crd(x) = 2sin(x / 2)
/// @tparam T Type of number
/// @param x Radians
/// @return Chord of x
template <typename T>
constexpr T Crd(T x) {
    const T two = 2;
    return Sin(x / two) * two;
}
/// @brief versin(x) = 1 - cos(x)
/// @tparam T Type of number
/// @param x Radians
/// @return Versed sine of x
template <typename T>
constexpr T Versin(T x) {
    const T one = 1;
    return one - Cos<T>(x);
}
/// @brief coversin(x) = 1 - sin(x)
/// @tparam T Type of number
/// @param x Radians
/// @return Coversed sine of x
template <typename T>
constexpr T Coversin(T x) {
    const T one = 1;
    return one - Sin(x);
}
/// @brief vercos(x) = 1 + cos(x)
/// @tparam T Type of number
/// @param x Radians
/// @return Versed cosine of x
template <typename T>
constexpr T Vercos(T x) {
    const T one = 1;
    return one + Cos<T>(x);
}
/// @brief covercos(x) = 1 + sin(x)
/// @tparam T Type of number
/// @param x Radians
/// @return Coversed cosine of x
template <typename T>
constexpr T Covercos(T x) {
    const T one = 1;
    return one + Sin(x);
}
/// @brief haversin(x) = (1 - cos(x)) / 2
/// @tparam T Type of number
/// @param x Radians
/// @return Haversed sine of x
template <typename T>
constexpr T Haversin(T x) {
    const T one = 1;
    const T two = 2;
    return (one - Cos<T>(x)) / two;
}
/// @brief hacoversin(x) = (1 - sin(x)) / 2
/// @tparam T Type of number
/// @param x Radians
/// @return Hacoversed sine of x
template <typename T>
constexpr T Hacoversin(T x) {
    const T one = 1;
    const T two = 2;
    return (one - Sin(x)) / two;
}
/// @brief havercos(x) = (1 + cos(x)) / 2
/// @tparam T Type of number
/// @param x Radians
/// @return Haversed cosine of x
template <typename T>
constexpr T Havercos(T x) {
    const T one = 1;
    const T two = 2;
    return (one + Cos<T>(x)) / two;
}
/// @brief hacovercos(x) = (1 + sin(x)) / 2
/// @tparam T Type of number
/// @param x Radians
/// @return Hacoversed cosine of x
template <typename T>
constexpr T Hacovercos(T x) {
    const T one = 1;
    const T two = 2;
    return (one + Sin(x)) / two;
}
/// @brief exsec(x) = versin(x) / cos(x)
/// @tparam T Type of number
/// @param x Radians
/// @return Exsecant of x
template <typename T>
constexpr T Exsec(T x) {
    return Versin<T>(x) / Cos<T>(x);
}
/// @brief excsc(x) = coversin(x) / sin(x)
/// @tparam T Type of number
/// @param x Radians
/// @return Excosecant of x
template <typename T>
constexpr T Excsc(T x) {
    return Coversin<T>(x) / Sin(x);
}
/// @brief sinh(x) = (exp(x) - exp(-x)) / 2
/// @tparam T Type of number
/// @param x Radians
/// @return Hyperbolic sine of x
template <typename T>
T HyperbolicSin(T x) {
    const T two = 2;
    return (Exp(x) - Exp(-x)) / two;
}
/// @brief cosh(x) = (exp(x) + exp(-x)) / 2
/// @tparam T Type of number
/// @param x Radians
/// @return Hyperbolic cosine of x
template <typename T>
T HyperbolicCos(T x) {
    const T two = 2;
    return (Exp(x) + Exp(-x)) / two;
}
/// @brief tanh(x) = sinh(x) / cosh(x)
/// @tparam T Type of number
/// @param x Radians
/// @return Hyperbolic tangent of x
template <typename T>
T HyperbolicTan(T x) {
    return HyperbolicSin<T>(x) / HyperbolicCos<T>(x);
}
/// @brief coth(x) = (exp(2x) + 1) / (exp(2x) - 1)
/// @tparam T Type of number
/// @param x Radians
/// @return Hyperbolic cotangent of x
template <typename T>
constexpr T HyperbolicCot(T x) {
    const T one = 1;
    const T two = 2;
    return (Exp(x * two) + one) / (Exp(x * two) - one);
}
/// @brief sech(x) = 2e^x / (e^2x + 1)
/// @tparam T Type of number
/// @param x Radians
/// @return Hyperbolic secant of x
template <typename T>
constexpr T HyperbolicSec(T x) {
    const T one = 1;
    const T two = 2;
    return (Exp(x) * two) / (Exp(x * two) + one);
}
/// @brief csch(x) = 2e^x / (e^2x - 1)
/// @tparam T Type of number
/// @param x Radians
/// @return Hyperbolic cosecant of x
template <typename T>
constexpr T HyperbolicCsc(T x) {
    const T one = 1;
    const T two = 2;
    return (Exp(x) * two) / (Exp(x * two) - one);
}
/// @brief arccos(x) = pi / 2 - arcsin(x)
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed sine of x
template <typename T>
constexpr T InversedCos(T x) {
    const T tmp = pi / 2;
    return tmp - InversedSin(x);
}
/// @brief arctan(x) = arcsin(x / sqrt(1 + x^2))
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed cotangent of x
template <typename T>
constexpr T InversedTan(T x) {
    const T one = 1;
    return InversedSin(x / Sqrt(one + Pow(x, 2)));
}
/// @brief arccot(x) = pi / 2 - arctan(x)
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed cotangent of x
template <typename T>
constexpr T InversedCot(T x) {
    const T tmp = pi / 2;
    return tmp - InversedTan<T>(x);
}
/// @brief arcsec(x) = arccos(1 / x)
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed secant of x
template <typename T>
constexpr T InversedSec(T x) {
    const T one = 1;
    return InversedCos<T>(one / x);
}
/// @brief arccsc(x) = arcsin(1 / x)
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed cosecant of x
template <typename T>
constexpr T InversedCsc(T x) {
    const T one = 1;
    return InversedSin(one / x);
}
/// @brief arccrd(x) = 2arcsin(x / 2)
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed chord of x
template <typename T>
constexpr T InversedCrd(T x) {
    const T two = 2;
    return InversedSin(x / two) * two;
}
/// @brief arcexsec(x) = arcsec(x + 1)
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed exsecant of x
template <typename T>
constexpr T InversedExsec(T x) {
    const T one = 1;
    return InversedSec<T>(x + one);
}
/// @brief arcexcsc(x) = arccsc(x + 1)
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed excosecant of x
template <typename T>
constexpr T InversedExcsc(T x) {
    const T one = 1;
    return InversedCsc<T>(x + one);
}
/// @brief arcversin(x) = arccos(1 - x)
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed versed sine of x
template <typename T>
constexpr T InversedVersin(T x) {
    const T one = 1;
    return InversedCos<T>(one - x);
}
/// @brief arccoversin(x) = arcsin(1 - x)
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed coversed sine of x
template <typename T>
constexpr T InversedCoversin(T x) {
    const T one = 1;
    return InversedSin(one - x);
}
/// @brief arcvercos(x) = arccos(x - 1)
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed versed cosine of x
template <typename T>
constexpr T InversedVercos(T x) {
    const T one = 1;
    return InversedCos<T>(x - one);
}
/// @brief arccovercos(x) = arcsin(x - 1)
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed coversed cosine of x
template <typename T>
constexpr T InversedCovercos(T x) {
    const T one = 1;
    return InversedSin(x - one);
}
/// @brief archaversin(x) = arccos(1 - 2x)
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed haversed sine of x
template <typename T>
constexpr T InversedHaversin(T x) {
    const T one = 1;
    const T two = 2;
    return InversedCos<T>(one - x * two);
}
/// @brief archacoversin(x) = arcsin(1 - 2x)
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed hacoversed sine of x
template <typename T>
constexpr T InversedHacoversin(T x) {
    const T one = 1;
    const T two = 2;
    return InversedSin(one - x * two);
}
/// @brief archavercos(x) = arccos(2x - 1)
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed haversed cosine of x
template <typename T>
constexpr T InversedHavercos(T x) {
    const T one = 1;
    const T two = 2;
    return InversedCos<T>(x * two - one);
}
/// @brief archacovercos(x) = arcsin(2x - 1)
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed hacoversed cosine of x
template <typename T>
constexpr T InversedHacovercos(T x) {
    const T one = 1;
    const T two = 2;
    return InversedSin(x * two - one);
}
/// @brief arcsinh(x) = ln(x + sqrt(x^2 + 1))
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed hyperbolic sine of x
template <typename T>
T InversedHyperbolicSin(T x) {
    const T one = 1;
    return NaturalLog(x + Sqrt(Pow(x, 2) + one));
}
/// @brief arccosh(x) = ln(x + sqrt(x^2 - 1))
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed hyperbolic cosine of x
template <typename T>
T InversedHyperbolicCos(T x) {
    const T one = 1;
    return NaturalLog(x + Sqrt(Pow(x, 2) - one));
}
/// @brief arctanh(x) = ln((1 + x) / (1 - x)) / 2
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed hyperbolic tangent of x
template <typename T>
T InversedHyperbolicTan(T x) {
    const T one = 1;
    const T two = 2;
    return NaturalLog((one + x) / (one - x)) / two;
}
/// @brief arccoth(x) = ln((x + 1) / (x - 1)) / 2
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed hyperbolic cotangent of x
template <typename T>
constexpr T InversedHyperbolicCot(T x) {
    const T one = 1;
    const T two = 2;
    return NaturalLog((x + one) / (x - one)) / two;
}
/// @brief arcsech(x) = ln((1 / x) + sqrt(2 $ (1 / x^2 - 1)))
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed hyperbolic secant of x
template <typename T>
constexpr T InversedHyperbolicSec(T x) {
    const T one = 1;
    return InversedHyperbolicCos<T>(one / x);
}
/// @brief arccsch(x) = ln((1 / x) + sqrt(2 $ (1 / x^2 + 1)))
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed hyperbolic cosecant of x
template <typename T>
constexpr T InversedHyperbolicCsc(T x) {
    const T one = 1;
    return NaturalLog(one / x + Sqrt(one / Pow(x, 2) + one));
}

#endif