#ifndef DualNumber_H
#define DualNumber_H
#include <Interfaces/Printable.hpp>
#include <Interfaces/Orderable.hpp>
#include <Host.hpp>

template <typename T>
struct DualNumber;
template <typename T>
struct DualNumber : MathLib::Orderable<DualNumber<T>>, MathLib::Printable {
    T real;
    T eps;

    DualNumber(void) : real(0), eps(0) {}
    DualNumber(const T& real) : real(real), eps(0) {}
    DualNumber(const T& real, const T& eps) : real(real), eps(eps) {}
    [[nodiscard]] DualNumber<T> operator+(const DualNumber<T>& other) const {
        return DualNumber<T>(real + other.real, eps + other.eps);
    }
    [[nodiscard]] DualNumber<T> operator-(const DualNumber<T>& other) const {
        return DualNumber<T>(real - other.real, eps - other.eps);
    }
    [[nodiscard]] DualNumber<T> operator*(const DualNumber<T>& other) const {
        return DualNumber<T>(real * other.real, real * other.eps + eps * other.real);
    }
    [[nodiscard]] DualNumber<T> operator/(const DualNumber<T>& other) const {
        if (MathLib::FloatsEqual<T>(other.real, 0)) return MathLib::FloatsEqual<T>(real, 0) ? DualNumber<T>(eps / other.eps, 0) : DualNumber<T>(MathLib::nan, MathLib::nan);
        return DualNumber<T>(real / other.real, (eps * other.real - real * other.eps) / (other.real * other.real));
    }
    DualNumber<T> operator-(void) const {
        return DualNumber(-real, -eps);
    }
    DualNumber<T>& operator+=(const DualNumber<T>& other) {
        real += other.real;
        eps += other.eps;
        return *this;
    }
    DualNumber<T>& operator-=(const DualNumber<T>& other) {
        real -= other.real;
        eps -= other.eps;
        return *this;
    }
    DualNumber<T>& operator*=(const DualNumber<T>& other) {
        const T tmp = real;
        real *= other.real;
        eps = tmp * other.eps + eps * other.real;
        return *this;
    }
    DualNumber<T>& operator/=(const DualNumber<T>& other) {
        return *this = *this / other;
    }
    [[nodiscard]] DualNumber<T> Exp(void) const {
        const T exp = MathLib::Exp(real);
        return DualNumber<T>(exp, exp * eps);
    }
    [[nodiscard]] DualNumber<T> Sigmoid(void) const {
        return DualNumber<T>(1) / (DualNumber<T>(1) + (-*this).Exp());
    }
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override {
        return MathLib::Formatter<T>().ToString(real, padding) + " + " + MathLib::Formatter<T>().ToString(eps) + "eps";
    }
    [[nodiscard]] virtual bool LessThanEqual(const DualNumber<T>& other) const override {
        return real < other.real || (MathLib::FloatsEqual<T>(real, other.real) && eps <= other.eps);
    }
};
namespace MathLib {
    MakeTemplatedFormatter(DualNumber<T>, self, padding, { return self.ToString(padding); }, typename T);
}
using dual_t = DualNumber<MathLib::num_t>;

#endif