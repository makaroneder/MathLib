#ifndef SequencesOfConstants_H
#define SequencesOfConstants_H
#include "Constants.hpp"
#include "Summation.hpp"
#include "Trigonometry.hpp"

template <typename T>
T HarmonicNumber(T n) {
    return Summation<T>(1, n, [](T k) {
        return 1 / k;
    });
}
template <typename T>
T BernoulliNumber(T t, bool neg = false) {
    return t / 2 * (HyperbolicCot<T>(t / 2) + (neg ? -1 : 1));
}
static constexpr num_t favardConstantsEnd = 100;
template <typename T>
T FavardConstants(T r) {
    return 4 / pi * Summation<T>(0, favardConstantsEnd, [r](T n) {
        return std::pow(std::pow(-1, n) / (2 * n + 1), r + 1);
    });
}
static constexpr num_t champernowneConstantsEnd = 100;
template <typename T>
T ChampernowneConstants(T b) {
    return Summation<T>(1, champernowneConstantsEnd, [b](T n) {
        return n / (b * Summation<T>(1, n, [b](T k) {
            return Log<T>(b, k + 1);
        }));
    });
}
template <typename T>
T BerahaConstants(T n) {
    return 2 + 2 * std::cos((2 * pi) / n);
}
template <typename T>
T HyperHarmonicNumber(T r, T n) {
    if (r == 0) return 1 / n;
    else return Summation<T>(1, n, [r](T k) {
        return HyperHarmonicNumber<T>(r - 1, k);
    });
}
static constexpr num_t gregoryNumberEnd = 100;
template <typename T>
T GregoryNumber(T x) {
    return Summation<T>(0, gregoryNumberEnd, [x](T n) {
        return std::pow(-1, n) / ((2 * n + 1) * std::pow(x, 2 * n + 1));
    });
}
template <typename T>
T MetallicMean(T n) {
    return (n + std::sqrt(std::pow(n, 2) + 4)) / 2;
}

#endif