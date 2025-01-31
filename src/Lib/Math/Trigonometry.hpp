#ifndef MathLib_Math_Trigonometry_H
#define MathLib_Math_Trigonometry_H
#include "Constants.hpp"
#include "Host.hpp"

// TODO: Use [[nodiscard]]
namespace MathLib {
    /// @brief sinc(x) = sin(x) / x
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Sinc of x
    template <typename T>
    T Sinc(const T& x) {
        StartBenchmark
        ReturnFromBenchmark(FloatsEqual<T>(x, 0) ? 1 : Sin(x) / x);
    }
    /// @brief cos(x) = sin(pi / 2 - x)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Cosine of x
    template <typename T>
    T Cos(const T& x) {
        StartBenchmark
        const T tmp = pi / 2;
        ReturnFromBenchmark(Sin(tmp - x));
    }
    /// @brief tan(x) = sin(x) / cos(x)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Tangent of x
    template <typename T>
    T Tan(const T& x) {
        StartBenchmark
        ReturnFromBenchmark(Sin(x) / Cos<T>(x));
    }
    /// @brief cot(x) = cos(x) / sin(x)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Cotangent of x
    template <typename T>
    constexpr T Cot(const T& x) {
        StartBenchmark
        ReturnFromBenchmark(Cos<T>(x) / Sin(x));
    }
    /// @brief sec(x) = 1 / cos(x)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Secant of x
    template <typename T>
    constexpr T Sec(const T& x) {
        StartBenchmark
        const T one = 1;
        ReturnFromBenchmark(one / Cos<T>(x));
    }
    /// @brief csc(x) = 1 / sin(x)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Cosecant of x
    template <typename T>
    constexpr T Csc(const T& x) {
        StartBenchmark
        const T one = 1;
        ReturnFromBenchmark(one / Sin(x));
    }
    /// @brief crd(x) = 2sin(x / 2)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Chord of x
    template <typename T>
    constexpr T Crd(const T& x) {
        StartBenchmark
        const T two = 2;
        ReturnFromBenchmark(Sin(x / two) * two);
    }
    /// @brief versin(x) = 1 - cos(x)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Versed sine of x
    template <typename T>
    constexpr T Versin(const T& x) {
        StartBenchmark
        const T one = 1;
        ReturnFromBenchmark(one - Cos<T>(x));
    }
    /// @brief coversin(x) = 1 - sin(x)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Coversed sine of x
    template <typename T>
    constexpr T Coversin(const T& x) {
        StartBenchmark
        const T one = 1;
        ReturnFromBenchmark(one - Sin(x));
    }
    /// @brief vercos(x) = 1 + cos(x)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Versed cosine of x
    template <typename T>
    constexpr T Vercos(const T& x) {
        StartBenchmark
        const T one = 1;
        ReturnFromBenchmark(one + Cos<T>(x));
    }
    /// @brief covercos(x) = 1 + sin(x)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Coversed cosine of x
    template <typename T>
    constexpr T Covercos(const T& x) {
        StartBenchmark
        const T one = 1;
        ReturnFromBenchmark(one + Sin(x));
    }
    /// @brief haversin(x) = (1 - cos(x)) / 2
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Haversed sine of x
    template <typename T>
    constexpr T Haversin(const T& x) {
        StartBenchmark
        const T one = 1;
        const T two = 2;
        ReturnFromBenchmark((one - Cos<T>(x)) / two);
    }
    /// @brief hacoversin(x) = (1 - sin(x)) / 2
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Hacoversed sine of x
    template <typename T>
    constexpr T Hacoversin(const T& x) {
        StartBenchmark
        const T one = 1;
        const T two = 2;
        ReturnFromBenchmark((one - Sin(x)) / two);
    }
    /// @brief havercos(x) = (1 + cos(x)) / 2
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Haversed cosine of x
    template <typename T>
    constexpr T Havercos(const T& x) {
        StartBenchmark
        const T one = 1;
        const T two = 2;
        ReturnFromBenchmark((one + Cos<T>(x)) / two);
    }
    /// @brief hacovercos(x) = (1 + sin(x)) / 2
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Hacoversed cosine of x
    template <typename T>
    constexpr T Hacovercos(const T& x) {
        StartBenchmark
        const T one = 1;
        const T two = 2;
        ReturnFromBenchmark((one + Sin(x)) / two);
    }
    /// @brief exsec(x) = versin(x) / cos(x)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Exsecant of x
    template <typename T>
    constexpr T Exsec(const T& x) {
        StartBenchmark
        ReturnFromBenchmark(Versin<T>(x) / Cos<T>(x));
    }
    /// @brief excsc(x) = coversin(x) / sin(x)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Excosecant of x
    template <typename T>
    constexpr T Excsc(const T& x) {
        StartBenchmark
        ReturnFromBenchmark(Coversin<T>(x) / Sin(x));
    }
    /// @brief sinh(x) = (exp(x) - exp(-x)) / 2
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Hyperbolic sine of x
    template <typename T>
    T HyperbolicSin(const T& x) {
        StartBenchmark
        const T two = 2;
        ReturnFromBenchmark((Exp(x) - Exp(-x)) / two);
    }
    /// @brief cosh(x) = (exp(x) + exp(-x)) / 2
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Hyperbolic cosine of x
    template <typename T>
    T HyperbolicCos(const T& x) {
        StartBenchmark
        const T two = 2;
        ReturnFromBenchmark((Exp(x) + Exp(-x)) / two);
    }
    /// @brief tanh(x) = sinh(x) / cosh(x)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Hyperbolic tangent of x
    template <typename T>
    T HyperbolicTan(const T& x) {
        StartBenchmark
        ReturnFromBenchmark(HyperbolicSin<T>(x) / HyperbolicCos<T>(x));
    }
    /// @brief coth(x) = (exp(2x) + 1) / (exp(2x) - 1)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Hyperbolic cotangent of x
    template <typename T>
    constexpr T HyperbolicCot(const T& x) {
        StartBenchmark
        const T one = 1;
        const T two = 2;
        ReturnFromBenchmark((Exp(x * two) + one) / (Exp(x * two) - one));
    }
    /// @brief sech(x) = 2e^x / (e^2x + 1)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Hyperbolic secant of x
    template <typename T>
    constexpr T HyperbolicSec(const T& x) {
        StartBenchmark
        const T one = 1;
        const T two = 2;
        ReturnFromBenchmark((Exp(x) * two) / (Exp(x * two) + one));
    }
    /// @brief csch(x) = 2e^x / (e^2x - 1)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Hyperbolic cosecant of x
    template <typename T>
    constexpr T HyperbolicCsc(const T& x) {
        StartBenchmark
        const T one = 1;
        const T two = 2;
        ReturnFromBenchmark((Exp(x) * two) / (Exp(x * two) - one));
    }
    /// @brief arccos(x) = pi / 2 - arcsin(x)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Inversed sine of x
    template <typename T>
    constexpr T InversedCos(const T& x) {
        StartBenchmark
        const T tmp = pi / 2;
        ReturnFromBenchmark(tmp - InversedSin(x));
    }
    /// @brief arctan(x) = arcsin(x / sqrt(1 + x^2))
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Inversed cotangent of x
    template <typename T>
    constexpr T InversedTan(const T& x) {
        StartBenchmark
        const T one = 1;
        ReturnFromBenchmark(InversedSin(x / Sqrt(one + x * x)));
    }
    /// @brief arccot(x) = pi / 2 - arctan(x)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Inversed cotangent of x
    template <typename T>
    constexpr T InversedCot(const T& x) {
        StartBenchmark
        const T tmp = pi / 2;
        ReturnFromBenchmark(tmp - InversedTan<T>(x));
    }
    /// @brief arcsec(x) = arccos(1 / x)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Inversed secant of x
    template <typename T>
    constexpr T InversedSec(const T& x) {
        StartBenchmark
        const T one = 1;
        ReturnFromBenchmark(InversedCos<T>(one / x));
    }
    /// @brief arccsc(x) = arcsin(1 / x)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Inversed cosecant of x
    template <typename T>
    constexpr T InversedCsc(const T& x) {
        StartBenchmark
        const T one = 1;
        ReturnFromBenchmark(InversedSin(one / x));
    }
    /// @brief arccrd(x) = 2arcsin(x / 2)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Inversed chord of x
    template <typename T>
    constexpr T InversedCrd(const T& x) {
        StartBenchmark
        const T two = 2;
        ReturnFromBenchmark(InversedSin(x / two) * two);
    }
    /// @brief arcexsec(x) = arcsec(x + 1)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Inversed exsecant of x
    template <typename T>
    constexpr T InversedExsec(const T& x) {
        StartBenchmark
        const T one = 1;
        ReturnFromBenchmark(InversedSec<T>(x + one));
    }
    /// @brief arcexcsc(x) = arccsc(x + 1)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Inversed excosecant of x
    template <typename T>
    constexpr T InversedExcsc(const T& x) {
        StartBenchmark
        const T one = 1;
        ReturnFromBenchmark(InversedCsc<T>(x + one));
    }
    /// @brief arcversin(x) = arccos(1 - x)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Inversed versed sine of x
    template <typename T>
    constexpr T InversedVersin(const T& x) {
        StartBenchmark
        const T one = 1;
        ReturnFromBenchmark(InversedCos<T>(one - x));
    }
    /// @brief arccoversin(x) = arcsin(1 - x)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Inversed coversed sine of x
    template <typename T>
    constexpr T InversedCoversin(const T& x) {
        StartBenchmark
        const T one = 1;
        ReturnFromBenchmark(InversedSin(one - x));
    }
    /// @brief arcvercos(x) = arccos(x - 1)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Inversed versed cosine of x
    template <typename T>
    constexpr T InversedVercos(const T& x) {
        StartBenchmark
        const T one = 1;
        ReturnFromBenchmark(InversedCos<T>(x - one));
    }
    /// @brief arccovercos(x) = arcsin(x - 1)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Inversed coversed cosine of x
    template <typename T>
    constexpr T InversedCovercos(const T& x) {
        StartBenchmark
        const T one = 1;
        ReturnFromBenchmark(InversedSin(x - one));
    }
    /// @brief archaversin(x) = arccos(1 - 2x)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Inversed haversed sine of x
    template <typename T>
    constexpr T InversedHaversin(const T& x) {
        StartBenchmark
        const T one = 1;
        const T two = 2;
        ReturnFromBenchmark(InversedCos<T>(one - x * two));
    }
    /// @brief archacoversin(x) = arcsin(1 - 2x)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Inversed hacoversed sine of x
    template <typename T>
    constexpr T InversedHacoversin(const T& x) {
        StartBenchmark
        const T one = 1;
        const T two = 2;
        ReturnFromBenchmark(InversedSin(one - x * two));
    }
    /// @brief archavercos(x) = arccos(2x - 1)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Inversed haversed cosine of x
    template <typename T>
    constexpr T InversedHavercos(const T& x) {
        StartBenchmark
        const T one = 1;
        const T two = 2;
        ReturnFromBenchmark(InversedCos<T>(x * two - one));
    }
    /// @brief archacovercos(x) = arcsin(2x - 1)
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Inversed hacoversed cosine of x
    template <typename T>
    constexpr T InversedHacovercos(const T& x) {
        StartBenchmark
        const T one = 1;
        const T two = 2;
        ReturnFromBenchmark(InversedSin(x * two - one));
    }
    /// @brief arcsinh(x) = ln(x + sqrt(x^2 + 1))
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Inversed hyperbolic sine of x
    template <typename T>
    T InversedHyperbolicSin(const T& x) {
        StartBenchmark
        const T one = 1;
        ReturnFromBenchmark(NaturalLog(x + Sqrt(x * x + one)));
    }
    /// @brief arccosh(x) = ln(x + sqrt(x^2 - 1))
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Inversed hyperbolic cosine of x
    template <typename T>
    T InversedHyperbolicCos(const T& x) {
        StartBenchmark
        const T one = 1;
        ReturnFromBenchmark(NaturalLog(x + Sqrt(x * x - one)));
    }
    /// @brief arctanh(x) = ln((1 + x) / (1 - x)) / 2
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Inversed hyperbolic tangent of x
    template <typename T>
    T InversedHyperbolicTan(const T& x) {
        StartBenchmark
        const T one = 1;
        const T two = 2;
        ReturnFromBenchmark(NaturalLog((one + x) / (one - x)) / two);
    }
    /// @brief arccoth(x) = ln((x + 1) / (x - 1)) / 2
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Inversed hyperbolic cotangent of x
    template <typename T>
    constexpr T InversedHyperbolicCot(const T& x) {
        StartBenchmark
        const T one = 1;
        const T two = 2;
        ReturnFromBenchmark(NaturalLog((x + one) / (x - one)) / two);
    }
    /// @brief arcsech(x) = ln((1 / x) + sqrt(2 $ (1 / x^2 - 1)))
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Inversed hyperbolic secant of x
    template <typename T>
    constexpr T InversedHyperbolicSec(const T& x) {
        StartBenchmark
        const T one = 1;
        ReturnFromBenchmark(InversedHyperbolicCos<T>(one / x));
    }
    /// @brief arccsch(x) = ln((1 / x) + sqrt(2 $ (1 / x^2 + 1)))
    /// @tparam T Type of number
    /// @param x Radians
    /// @return Inversed hyperbolic cosecant of x
    template <typename T>
    constexpr T InversedHyperbolicCsc(const T& x) {
        StartBenchmark
        const T one = 1;
        ReturnFromBenchmark(NaturalLog(one / x + Sqrt(one / (x * x) + one)));
    }
}

#endif