#ifndef MathLib_Math_FourierTransform_H
#define MathLib_Math_FourierTransform_H
#include "FourierTransformData.hpp"
#include "Integral.hpp"

namespace MathLib {
    template <typename T>
    Complex<T> IntegralFourierTransform(const void* func, T t) {
        StartBenchmark
        const FourierTransformData<T>* data = (const FourierTransformData<T>*)func;
        const T tmp = 2 * pi * data->x * t;
        ReturnFromBenchmark((*data->func)(t) * Exp(Complex<T>(0, data->inverse ? tmp : -tmp)));
    }
    template <typename T>
    struct FourierTransform : Function<Complex<T>, T> {
        FourierTransform(Function<Complex<T>, T>* function, bool inverse, size_t n, IntegrationType type) : Function<Complex<T>, T>(nullptr), function(function), inverse(inverse), n(n), type(type) {
            EmptyBenchmark
        }
        ~FourierTransform(void) {
            StartBenchmark
            delete function;
            EndBenchmark
        }
        [[nodiscard]] virtual Complex<T> Invoke(const void*, T k) const override {
            StartBenchmark
            const FourierTransformData<T> data = FourierTransformData<T>(function, k, inverse);
            ReturnFromBenchmark((Integrate<Complex<T>, T>(FunctionPointer<Complex<T>, T>((void*)&data, &IntegralFourierTransform<T>), -MakeInf(), MakeInf(), n, type)));
        }

        private:
        Function<Complex<T>, T>* function;
        bool inverse;
        size_t n;
        IntegrationType type;
    };
}

#endif