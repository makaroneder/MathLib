#ifndef MathLib_Math_Quaternion_H
#define MathLib_Math_Quaternion_H
#include "Vector.hpp"
#include "Trigonometry.hpp"

namespace MathLib {
    template <typename T>
    struct Quaternion : Printable {
        CreateOperators(Quaternion<T>, T)
        CreateExponential(Quaternion<T>, true, Quaternion<T>(1, 0, 0, 0))
        Quaternion(const T& a_, const T& b_, const T& c_, const T& d_) : a(a_), b(b_), c(c_), d(d_) {}
        Quaternion(const T& scalar, const Matrix<T>& vector) : a(scalar), b(GetX(vector)), c(GetY(vector)), d(GetZ(vector)) {}
        [[nodiscard]] T GetScalar(void) const {
            return a;
        }
        [[nodiscard]] Matrix<T> GetVector(void) const {
            return CreateVector<T>(b, c, d);
        }
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override {
            String ret = FloatsEqual<T>(a, 0) ? "" : Formatter<T>::ToString(a);
            if (FloatsEqual<T>(b, 1)) ret += ret.IsEmpty() ? "i" : " + i";
            else if (!FloatsEqual<T>(b, 0)) ret += String(ret.IsEmpty() ? "" : " + ") + Formatter<T>::ToString(b) + 'i';
            if (FloatsEqual<T>(c, 1)) ret += ret.IsEmpty() ? "j" : " + j";
            else if (!FloatsEqual<T>(c, 0)) ret += String(ret.IsEmpty() ? "" : " + ") + Formatter<T>::ToString(c) + 'j';
            if (FloatsEqual<T>(d, 1)) ret += ret.IsEmpty() ? "k" : " + k";
            else if (!FloatsEqual<T>(d, 0)) ret += String(ret.IsEmpty() ? "" : " + ") + Formatter<T>::ToString(d) + 'k';
            return CollectionToString(padding) + (ret.IsEmpty() ? "0" : ret);
        }
        [[nodiscard]] Quaternion<T> operator*(const Quaternion<T>& other) const {
            return Quaternion<T>(
                a * other.a - b * other.b - c * other.c - d * other.d,
                a * other.b + b * other.a + c * other.d - d * other.c,
                a * other.c - b * other.d + c * other.a + d * other.b,
                a * other.d + b * other.c - c * other.b + d * other.a
            );
        }
        [[nodiscard]] T GetNormSquared(void) const {
            return a * a + b * b + c * c + d * d;
        }
        [[nodiscard]] T GetNorm(void) const {
            return Sqrt(GetNormSquared());
        }
        [[nodiscard]] Expected<Quaternion<T>> Log(void) const {
            const Matrix<T> v = GetVector();
            const T norm = GetNorm();
            return FloatsEqual<T>(norm, 0) ? Expected<Quaternion<T>>() : Expected<Quaternion<T>>(Quaternion<T>(NaturalLog(norm), v.Normalize() * InversedCos<T>(a / norm)));
        }
        [[nodiscard]] Quaternion<T> Exponential(void) const {
            const Matrix<T> v = GetVector();
            const T len = v.GetLength();
            return Quaternion<T>(Cos<T>(len), v.Normalize() * Sin(len)) * Exp(a);
        }
        [[nodiscard]] Expected<Quaternion<T>> Pow(const T& n) const {
            if (n < 0) {
                const Expected<Quaternion<T>> tmp = GetInverse();
                return tmp.HasValue() ? tmp.Get().Pow(-n) : Expected<Quaternion<T>>();
            }
            const Expected<Quaternion<T>> tmp = Log();
            return tmp.HasValue() ? Expected<Quaternion<T>>((tmp.Get() * n).Exponential()) : Expected<Quaternion<T>>();
        }
        [[nodiscard]] Quaternion<T> Pow(const Quaternion<T>& n) const {
            const Expected<Quaternion<T>> tmp = Log();
            return tmp.HasValue() ? Expected<Quaternion<T>>((tmp.Get() * n).Exponential()) : Expected<Quaternion<T>>();
        }
        [[nodiscard]] Expected<Quaternion<T>> GetInverse(void) const {
            const T tmp = GetNormSquared();
            return FloatsEqual<T>(tmp, 0) ? Expected<Quaternion<T>>() : Expected<Quaternion<T>>(Quaternion<T>(a, -b, -c, -d) / tmp);
        }

        private:
        void Add(const Quaternion<T>& other) {
            a += other.a;
            b += other.b;
            c += other.c;
            d += other.d;
        }
        void Multiply(const T& scalar) {
            a *= scalar;
            b *= scalar;
            c *= scalar;
            d *= scalar;
        }
        T a;
        T b;
        T c;
        T d;
    };
    template <typename T>
    [[nodiscard]] Matrix<T> RotateVector(const Matrix<T>& point, const Matrix<T>& origin, const Matrix<T>& rotation) {
        const T angle = rotation.GetLength();
        if (FloatsEqual<T>(angle, 0)) return point;
        const Quaternion<T> quaternion = Quaternion<T>(Cos<T>(angle / 2), rotation.Normalize() * Sin(angle / 2));
        return (quaternion * Quaternion<T>(0, point - origin) * Quaternion<T>(quaternion.GetScalar(), -quaternion.GetVector())).GetVector() + origin;
    }
    template <typename T>
    [[nodiscard]] Matrix<T> CrossProduct(const Matrix<T>& a, const Matrix<T>& b) {
        return (Quaternion<T>(0, a) * Quaternion<T>(0, b)).GetVector();
    }
}

#endif