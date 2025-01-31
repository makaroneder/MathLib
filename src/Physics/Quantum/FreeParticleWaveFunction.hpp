#ifndef FreeParticleWaveFunction_H
#define FreeParticleWaveFunction_H
#include <Libc/HostFunction.hpp>
#include <Physics/Constants.hpp>
#include <Math/Integral.hpp>

template <typename T>
struct FreeParticleWaveFunction : MathLib::Function<MathLib::Complex<T>, T, T> {
    FreeParticleWaveFunction(MathLib::Function<MathLib::Complex<T>, T>* transform, const T& m, size_t n, MathLib::IntegrationType type) : MathLib::Function<MathLib::Complex<T>, T, T>(nullptr), transform(transform), m(m), n(n), type(type) {}
    virtual ~FreeParticleWaveFunction(void) override {
        delete transform;
    }
    [[nodiscard]] virtual MathLib::Complex<T> Invoke(const void*, T x, T t) const override {
        return MathLib::Integrate<MathLib::Complex<T>, T>(MathLib::HostFunction<MathLib::Complex<T>, T>([this, &x, &t] (T k) -> MathLib::Complex<T> {
            return (*transform)(k) * MathLib::Exp(MathLib::Complex<T>(0, k * x - (t * MathLib::reducedPlanckConstant.GetValue() * k * k / (2 * m))));
        }), -MathLib::MakeInf(), MathLib::MakeInf(), n, type) / MathLib::Sqrt(2 * MathLib::pi);
    }

    private:
    MathLib::Function<MathLib::Complex<T>, T>* transform;
    T m;
    size_t n;
    MathLib::IntegrationType type;
};

#endif