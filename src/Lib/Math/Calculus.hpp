#ifndef MathLib_Math_Calculus_H
#define MathLib_Math_Calculus_H
#include "../FunctionPointer.hpp"
#include "Matrix.hpp"

namespace MathLib {
    template <typename Ret, typename T>
    struct GradientData {
        const Function<Ret, Matrix<T>>& f;
        Matrix<T> args;
        size_t x;
        size_t y;

        GradientData(const Function<Ret, Matrix<T>>& f, const Matrix<T>& args, size_t x, size_t y) : f(f), args(args), x(x), y(y) {
            EmptyBenchmark
        }
    };
    template <typename Ret, typename T>
    Ret Derivative(const Function<Ret, T>& f, const T& x) {
        StartBenchmark
        const T h = eps * eps;
        ReturnFromBenchmark((f(x + h) - f(x)) / h);
    }
    template <typename Ret, typename T>
    Ret GradientFunc(const void* data, T x) {
        StartBenchmark
        const GradientData<Ret, T>* d = (const GradientData<Ret, T>*)data;
        Matrix<T> tmp = d->args;
        tmp.At(d->x, d->y) += x;
        ReturnFromBenchmark(d->f(tmp));
    }
    template <typename Ret, typename T>
    Matrix<Ret> Gradient(const Function<Ret, Matrix<T>>& f, const Matrix<T>& args) {
        StartBenchmark
        Matrix<Ret> ret = Matrix<Ret>(args.GetWidth(), args.GetHeight());
        for (size_t y = 0; y < ret.GetHeight(); y++) {
            for (size_t x = 0; x < ret.GetWidth(); x++) {
                GradientData<Ret, T> data = GradientData<Ret, T>(f, args, x, y);
                ret.At(x, y) = Derivative<Ret, T>(FunctionPointer<Ret, T>(&data, &GradientFunc<Ret, T>), 0);
            }
        }
        ReturnFromBenchmark(ret);
    }
}

#endif