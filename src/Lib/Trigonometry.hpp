#ifndef Trigonometry_H
#define Trigonometry_H
#include "Log.hpp"

/// @brief a * pi / 180
/// @tparam T Type of number
/// @param angle Angle in degrees to convert into radians
/// @return Angle represented in radians
template <typename T>
constexpr T DegreeToRadian(T a) {
    return a * (M_PI_4l / 45);
}
/// @brief cot(x) = cos(x) / sin(x)
/// @tparam T Type of number
/// @param x Radians
/// @return Cotangent of x
template <typename T>
constexpr T Cot(T x) {
    return std::cos(x) / std::sin(x);
}
/// @brief sec(x) = 1 / cos(x)
/// @tparam T Type of number
/// @param x Radians
/// @return Secant of x
template <typename T>
constexpr T Sec(T x) {
    return 1 / std::cos(x);
}
/// @brief csc(x) = 1 / sin(x)
/// @tparam T Type of number
/// @param x Radians
/// @return Cosecant of x
template <typename T>
constexpr T Csc(T x) {
    return 1 / std::sin(x);
}
/// @brief crd(x) = 2sin(x / 2)
/// @tparam T Type of number
/// @param x Radians
/// @return Chord of x
template <typename T>
constexpr T Crd(T x) {
    return 2 * std::sin(x / 2);
}
/// @brief versin(x) = 1 - cos(x)
/// @tparam T Type of number
/// @param x Radians
/// @return Versed sine of x
template <typename T>
constexpr T Versin(T x) {
    return 1 - std::cos(x);
}
/// @brief coversin(x) = 1 - sin(x)
/// @tparam T Type of number
/// @param x Radians
/// @return Coversed sine of x
template <typename T>
constexpr T Coversin(T x) {
    return 1 - std::sin(x);
}
/// @brief vercos(x) = 1 + cos(x)
/// @tparam T Type of number
/// @param x Radians
/// @return Versed cosine of x
template <typename T>
constexpr T Vercos(T x) {
    return 1 + std::cos(x);
}
/// @brief covercos(x) = 1 + sin(x)
/// @tparam T Type of number
/// @param x Radians
/// @return Coversed cosine of x
template <typename T>
constexpr T Covercos(T x) {
    return 1 + std::sin(x);
}
/// @brief haversin(x) = (1 - cos(x)) / 2
/// @tparam T Type of number
/// @param x Radians
/// @return Haversed sine of x
template <typename T>
constexpr T Haversin(T x) {
    return (1 - std::cos(x)) / 2;
}
/// @brief hacoversin(x) = (1 - sin(x)) / 2
/// @tparam T Type of number
/// @param x Radians
/// @return Hacoversed sine of x
template <typename T>
constexpr T Hacoversin(T x) {
    return (1 - std::sin(x)) / 2;
}
/// @brief havercos(x) = (1 + cos(x)) / 2
/// @tparam T Type of number
/// @param x Radians
/// @return Haversed cosine of x
template <typename T>
constexpr T Havercos(T x) {
    return (1 + std::cos(x)) / 2;
}
/// @brief hacovercos(x) = (1 + sin(x)) / 2
/// @tparam T Type of number
/// @param x Radians
/// @return Hacoversed cosine of x
template <typename T>
constexpr T Hacovercos(T x) {
    return (1 + std::sin(x)) / 2;
}
/// @brief exsec(x) = versin(x) / cos(x)
/// @tparam T Type of number
/// @param x Radians
/// @return Exsecant of x
template <typename T>
constexpr T Exsec(T x) {
    return Versin<T>(x) / std::cos(x);
}
/// @brief excsc(x) = coversin(x) / sin(x)
/// @tparam T Type of number
/// @param x Radians
/// @return Excosecant of x
template <typename T>
constexpr T Excsc(T x) {
    return Coversin<T>(x) / std::sin(x);
}
/// @brief coth(x) = (e^2x + 1) / (e^2x - 1)
/// @tparam T Type of number
/// @param x Radians
/// @return Hyperbolic cotangent of x
template <typename T>
constexpr T HyperbolicCot(T x) {
    return (exp(2 * x) + 1) / (exp(2 * x) - 1);
}
/// @brief sech(x) = 2e^x / (e^2x + 1)
/// @tparam T Type of number
/// @param x Radians
/// @return Hyperbolic secant of x
template <typename T>
constexpr T HyperbolicSec(T x) {
    return (exp(x) * 2) / (exp(2 * x) + 1);
}
/// @brief csch(x) = 2e^x / (e^2x - 1)
/// @tparam T Type of number
/// @param x Radians
/// @return Hyperbolic cosecant of x
template <typename T>
constexpr T HyperbolicCsc(T x) {
    return (exp(x) * 2) / (exp(2 * x) - 1);
}
/// @brief arccot(x) = pi / 2 - arctan(x)
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed cotangent of x
template <typename T>
constexpr T InversedCot(T x) {
    return M_PI_2l - std::atan(x);
}
/// @brief arcsec(x) = arccos(1 / x)
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed secant of x
template <typename T>
constexpr T InversedSec(T x) {
    return std::acos(1 / x);
}
/// @brief arccsc(x) = arcsin(1 / x)
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed cosecant of x
template <typename T>
constexpr T InversedCsc(T x) {
    return std::asin(1 / x);
}
/// @brief arccrd(x) = 2arcsin(x / 2)
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed chord of x
template <typename T>
constexpr T InversedCrd(T x) {
    return 2 * std::asin(x / 2);
}
/// @brief arcexsec(x) = arcsec(x + 1)
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed exsecant of x
template <typename T>
constexpr T InversedExsec(T x) {
    return InversedSec<T>(x + 1);
}
/// @brief arcexcsc(x) = arccsc(x + 1)
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed excosecant of x
template <typename T>
constexpr T InversedExcsc(T x) {
    return InversedCsc<T>(x + 1);
}
/// @brief arcversin(x) = arccos(1 - x)
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed versed sine of x
template <typename T>
constexpr T InversedVersin(T x) {
    return std::acos(1 - x);
}
/// @brief arccoversin(x) = arcsin(1 - x)
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed coversed sine of x
template <typename T>
constexpr T InversedCoversin(T x) {
    return std::asin(1 - x);
}
/// @brief arcvercos(x) = arccos(x - 1)
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed versed cosine of x
template <typename T>
constexpr T InversedVercos(T x) {
    return std::acos(x - 1);
}
/// @brief arccovercos(x) = arcsin(x - 1)
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed coversed cosine of x
template <typename T>
constexpr T InversedCovercos(T x) {
    return std::asin(x - 1);
}
/// @brief archaversin(x) = arccos(1 - 2x)
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed haversed sine of x
template <typename T>
constexpr T InversedHaversin(T x) {
    return std::acos(1 - 2 * x);
}
/// @brief archacoversin(x) = arcsin(1 - 2x)
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed hacoversed sine of x
template <typename T>
constexpr T InversedHacoversin(T x) {
    return std::asin(1 - 2 * x);
}
/// @brief archavercos(x) = arccos(2x - 1)
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed haversed cosine of x
template <typename T>
constexpr T InversedHavercos(T x) {
    return std::acos(2 * x - 1);
}
/// @brief archacovercos(x) = arcsin(2x - 1)
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed hacoversed cosine of x
template <typename T>
constexpr T InversedHacovercos(T x) {
    return std::asin(2 * x - 1);
}
/// @brief arccoth(x) = ln((x + 1) / (x - 1)) / 2
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed hyperbolic cotangent of x
template <typename T>
constexpr T InversedHyperbolicCot(T x) {
    return Ln<T>((x + 1) / (x - 1)) / 2;
}
/// @brief arcsech(x) = ln((1 / x) + sqrt(2 $ (1 / x^2 - 1)))
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed hyperbolic secant of x
template <typename T>
constexpr T InversedHyperbolicSec(T x) {
    return std::acosh(1 / x);
}
/// @brief arccsch(x) = ln((1 / x) + sqrt(2 $ (1 / x^2 + 1)))
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed hyperbolic cosecant of x
template <typename T>
constexpr T InversedHyperbolicCsc(T x) {
    return Ln<T>(1 / x + std::sqrt(1 / std::pow(x, 2) + 1));
}

#endif