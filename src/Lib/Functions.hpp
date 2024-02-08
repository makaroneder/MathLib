#ifndef Functions_H
#define Functions_H
#include "Summation.hpp"
#include <cmath>

#define ConstFunction(T, c)                         \
[c](T) {                                            \
    return c;                                       \
}
#define IdentityFunction(T)                         \
[](T x) {                                           \
    return x;                                       \
}
#define SignumFunction(T)                           \
[](T x) {                                           \
    return x == 0 ? 0 : (x / abs(x));               \
}
#define PolynomialFunction(T, a)                    \
[a](T x) {                                          \
    return Summation<T>(0, a.size(), [a, x](T n) {  \
        return a[n] * pow(x, n);                    \
    });                                             \
}
#define RationalFunction(T, p, q)                   \
[p, q](T x) {                                       \
    return p(x) / q(x);                             \
}
#define DerivativeFunction(T, f)                    \
[f](T x) {                                          \
    const T h = 1 / pointMultiplier;                \
    return (f(x + h) - f(x)) / h;                   \
}

#endif