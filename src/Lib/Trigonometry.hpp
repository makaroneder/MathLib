#ifndef Trigonometry_H
#define Trigonometry_H
#include <cmath>

/// @brief a * pi / 180
/// @tparam T Type of number
/// @param angle Angle in degrees to convert into radians
/// @return Angle represented in radians
template <typename T>
constexpr T DegreeToRadian(T angle) {
    return angle * (M_PI_4l / 45);
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
/// @brief exsec(x) = sec(x) - 1
/// @tparam T Type of number
/// @param x Radians
/// @return Exsecant of x
template <typename T>
constexpr T Exsec(T x) {
    return Sec<T>(x) - 1;
}
/// @brief excsc(x) = csc(x) - 1
/// @tparam T Type of number
/// @param x Radians
/// @return Excosecant of x
template <typename T>
constexpr T Excsc(T x) {
    return Csc<T>(x) - 1;
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

#endif