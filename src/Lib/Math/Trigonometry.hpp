#ifndef MathLib_Math_Trigonometry_H
#define MathLib_Math_Trigonometry_H
#include "Constants.hpp"
#include "Host.hpp"

namespace MathLib {
    template <typename T>
    [[nodiscard]] T Sinc(const T& x) {
        return FloatsEqual<T>(x, 0) ? 1 : Sin(x) / x;
    }
    template <typename T>
    [[nodiscard]] T Cos(const T& x) {
        return Sin(T(pi / 2) - x);
    }
    template <typename T>
    [[nodiscard]] T Tan(const T& x) {
        return Sin(x) / Cos<T>(x);
    }
    template <typename T>
    [[nodiscard]] constexpr T Cot(const T& x) {
        return Cos<T>(x) / Sin(x);
    }
    template <typename T>
    [[nodiscard]] constexpr T Sec(const T& x) {
        return T(1) / Cos<T>(x);
    }
    template <typename T>
    [[nodiscard]] constexpr T Csc(const T& x) {
        return T(1) / Sin(x);
    }
    template <typename T>
    [[nodiscard]] constexpr T Crd(const T& x) {
        return Sin(x / 2) * 2;
    }
    template <typename T>
    [[nodiscard]] constexpr T Versin(const T& x) {
        return T(1) - Cos<T>(x);
    }
    template <typename T>
    [[nodiscard]] constexpr T Coversin(const T& x) {
        return T(1) - Sin(x);
    }
    template <typename T>
    [[nodiscard]] constexpr T Vercos(const T& x) {
        return T(1) + Cos<T>(x);
    }
    template <typename T>
    [[nodiscard]] constexpr T Covercos(const T& x) {
        return T(1) + Sin(x);
    }
    template <typename T>
    [[nodiscard]] constexpr T Haversin(const T& x) {
        return (T(1) - Cos<T>(x)) / 2;
    }
    template <typename T>
    [[nodiscard]] constexpr T Hacoversin(const T& x) {
        return (T(1) - Sin(x)) / 2;
    }
    template <typename T>
    [[nodiscard]] constexpr T Havercos(const T& x) {
        return (T(1) + Cos<T>(x)) / 2;
    }
    template <typename T>
    [[nodiscard]] constexpr T Hacovercos(const T& x) {
        return (T(1) + Sin(x)) / 2;
    }
    template <typename T>
    [[nodiscard]] constexpr T Exsec(const T& x) {
        return Versin<T>(x) / Cos<T>(x);
    }
    template <typename T>
    [[nodiscard]] constexpr T Excsc(const T& x) {
        return Coversin<T>(x) / Sin(x);
    }
    template <typename T>
    [[nodiscard]] T HyperbolicSin(const T& x) {
        return (Exp(x) - Exp(-x)) / 2;
    }
    template <typename T>
    [[nodiscard]] T HyperbolicCos(const T& x) {
        return (Exp(x) + Exp(-x)) / 2;
    }
    template <typename T>
    [[nodiscard]] T HyperbolicTan(const T& x) {
        return HyperbolicSin<T>(x) / HyperbolicCos<T>(x);
    }
    template <typename T>
    [[nodiscard]] constexpr T HyperbolicCot(const T& x) {
        return (Exp(x * 2) + 1) / (Exp(x * 2) - 1);
    }
    template <typename T>
    [[nodiscard]] constexpr T HyperbolicSec(const T& x) {
        return (Exp(x) * 2) / (Exp(x * 2) + 1);
    }
    template <typename T>
    [[nodiscard]] constexpr T HyperbolicCsc(const T& x) {
        return (Exp(x) * 2) / (Exp(x * 2) - 1);
    }
    template <typename T>
    [[nodiscard]] constexpr T InversedCos(const T& x) {
        return T(pi / 2) - InversedSin(x);
    }
    template <typename T>
    [[nodiscard]] constexpr T InversedTan(const T& x) {
        return InversedSin(x / Sqrt(x * x + 1));
    }
    template <typename T>
    [[nodiscard]] constexpr T InversedCot(const T& x) {
        return T(pi / 2) - InversedTan<T>(x);
    }
    template <typename T>
    [[nodiscard]] constexpr T InversedSec(const T& x) {
        return InversedCos<T>(T(1) / x);
    }
    template <typename T>
    [[nodiscard]] constexpr T InversedCsc(const T& x) {
        return InversedSin(T(1) / x);
    }
    template <typename T>
    [[nodiscard]] constexpr T InversedCrd(const T& x) {
        return InversedSin(x / 2) * 2;
    }
    template <typename T>
    [[nodiscard]] constexpr T InversedExsec(const T& x) {
        return InversedSec<T>(x + 1);
    }
    template <typename T>
    [[nodiscard]] constexpr T InversedExcsc(const T& x) {
        return InversedCsc<T>(x + 1);
    }
    template <typename T>
    [[nodiscard]] constexpr T InversedVersin(const T& x) {
        return InversedCos<T>(T(1) - x);
    }
    template <typename T>
    [[nodiscard]] constexpr T InversedCoversin(const T& x) {
        return InversedSin(T(1) - x);
    }
    template <typename T>
    [[nodiscard]] constexpr T InversedVercos(const T& x) {
        return InversedCos<T>(x - 1);
    }
    template <typename T>
    [[nodiscard]] constexpr T InversedCovercos(const T& x) {
        return InversedSin(x - 1);
    }
    template <typename T>
    [[nodiscard]] constexpr T InversedHaversin(const T& x) {
        return InversedCos<T>(T(1) - x * 2);
    }
    template <typename T>
    [[nodiscard]] constexpr T InversedHacoversin(const T& x) {
        return InversedSin(T(1) - x * 2);
    }
    template <typename T>
    [[nodiscard]] constexpr T InversedHavercos(const T& x) {
        return InversedCos<T>(x * 2 - 1);
    }
    template <typename T>
    [[nodiscard]] constexpr T InversedHacovercos(const T& x) {
        return InversedSin(x * 2 - 1);
    }
    template <typename T>
    [[nodiscard]] T InversedHyperbolicSin(const T& x) {
        return NaturalLog(x + Sqrt(x * x + 1));
    }
    template <typename T>
    [[nodiscard]] T InversedHyperbolicCos(const T& x) {
        return NaturalLog(x + Sqrt(x * x - 1));
    }
    template <typename T>
    [[nodiscard]] T InversedHyperbolicTan(const T& x) {
        return NaturalLog((x + 1) / (T(1) - x)) / 2;
    }
    template <typename T>
    [[nodiscard]] constexpr T InversedHyperbolicCot(const T& x) {
        return NaturalLog((x + 1) / (x - 1)) / 2;
    }
    template <typename T>
    [[nodiscard]] constexpr T InversedHyperbolicSec(const T& x) {
        return InversedHyperbolicCos<T>(T(1) / x);
    }
    template <typename T>
    [[nodiscard]] constexpr T InversedHyperbolicCsc(const T& x) {
        return NaturalLog(T(1) / x + Sqrt(T(1) / (x * x) + 1));
    }
}

#endif