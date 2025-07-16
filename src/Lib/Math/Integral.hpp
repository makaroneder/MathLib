#ifndef MathLib_Math_Integral_H
#define MathLib_Math_Integral_H
#include "Trigonometry.hpp"
#include "../FunctionPointer.hpp"

namespace MathLib {
    enum class IntegrationType {
        TrapezoidalRule,
        SimpsonRule,
    };
    template <typename Ret, typename T>
    Ret InfiniteIntegralSubstitution(const void* func, T t) {
        StartBenchmark
        const T tmp = 1 - t * t;
        ReturnFromBenchmark((*(const Function<Ret, T>*)func)(t / tmp) * (1 + t * t) / (tmp * tmp));
    }
    template <typename Ret, typename T>
    Ret Integrate(const Function<Ret, T>& func, const T& a, const T& b, size_t n, IntegrationType type) {
        StartBenchmark
        if (IsInf(a) && IsInf(b)) ReturnFromBenchmark(Integrate(FunctionPointer<Ret, T>((void*)&func, &InfiniteIntegralSubstitution<Ret, T>), -1 + eps, 1 - eps, n, type));
        switch (type) {
            case IntegrationType::TrapezoidalRule: {
                const T h = (b - a) / n;
                Ret ret = (func(a) + func(b)) / 2;
                for (size_t i = 1; i < n; i++) ret += func(a + i * h);
                ReturnFromBenchmark(ret * h);
            }
            case IntegrationType::SimpsonRule: {
                if (n % 2) n++;
                const T h = (b - a) / n;
                Ret ret = (func(a) + func(b)) / 2;
                for (size_t i = 1; i < n; i += 2) ret += func(a + i * h) * 2;
                for (size_t i = 2; i < n - 1; i += 2) ret += func(a + i * h);
                ReturnFromBenchmark(ret * h / 1.5);
            }
            default: ReturnFromBenchmark(Ret());
        }
    }
}

#endif