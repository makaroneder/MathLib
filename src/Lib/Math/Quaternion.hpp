#ifndef MathLib_Math_Quaternion_H
#define MathLib_Math_Quaternion_H
#include "Vector.hpp"
#include "Trigonometry.hpp"

namespace MathLib {
    /// @brief q = a + bi + cj + dk
    /// @tparam T Type of number
    template <typename T>
    struct Quaternion : Printable {
        CreateOperators(Quaternion<T>, T)
        CreateExponential(Quaternion<T>, true, Quaternion<T>(1, 0, 0, 0))
        /// @brief q = a + bi + cj + dk
        /// @param a_ a
        /// @param b_ b
        /// @param c_ c
        /// @param d_ d
        Quaternion(const T& a_, const T& b_, const T& c_, const T& d_) : a(a_), b(b_), c(c_), d(d_) {}
        /// @brief q = a + v_0i + v_1j + v_2k
        /// @param scalar Scalar part of quaternion
        /// @param vector Vector part of quaternion
        Quaternion(const T& scalar, const Matrix<T>& vector) : a(scalar), b(GetX(vector)), c(GetY(vector)), d(GetZ(vector)) {}
        /// @brief Returns scalar part of quaternion
        /// @return Scalar part of quaternion
        [[nodiscard]] T GetScalar(void) const {
            return a;
        }
        /// @brief Returns vector part of quaternion
        /// @return Vector part of quaternion
        [[nodiscard]] Matrix<T> GetVector(void) const {
            return CreateVector<T>(b, c, d);
        }
        /// @brief Converts quaternion to string
        /// @param padding String to pad with
        /// @return String representation
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
        /// @brief (a + bi + cj + dk) * (e + fi + gj + hk) = (ae - bf - cg - dh) + (af + be + ch - dg)i + (ag - bh + ce + df)j + (ah + bg - cf + de)k
        /// @param other Other quaternion
        /// @return New quaternion
        [[nodiscard]] Quaternion<T> operator*(const Quaternion<T>& other) const {
            return Quaternion<T>(
                a * other.a - b * other.b - c * other.c - d * other.d,
                a * other.b + b * other.a + c * other.d - d * other.c,
                a * other.c - b * other.d + c * other.a + d * other.b,
                a * other.d + b * other.c - c * other.b + d * other.a
            );
        }
        /// @brief |q|^2 = a^2 + b^2 + c^2 + d^2
        /// @return Squared norm of quaternion
        [[nodiscard]] T GetNormSquared(void) const {
            return a * a + b * b + c * c + d * d;
        }
        /// @brief |q| = sqrt(a^2 + b^2 + c^2 + d^2)
        /// @return Norm of quaternion
        [[nodiscard]] T GetNorm(void) const {
            return Sqrt(GetNormSquared());
        }
        /// @brief ln(q) = ln(|q|) + v / |v| * arccos(a / |q|)
        /// @return Logarithm of quaternion
        [[nodiscard]] Expected<Quaternion<T>> Log(void) const {
            const Matrix<T> v = GetVector();
            const T norm = GetNorm();
            return FloatsEqual<T>(norm, 0) ? Expected<Quaternion<T>>() : Expected<Quaternion<T>>(Quaternion<T>(NaturalLog(norm), v.Normalize() * InversedCos<T>(a / norm)));
        }
        /// @brief e^q = e^a * (cos(|v|) + v / |v| * sin(|v|))
        /// @return Exponential of quaternion
        [[nodiscard]] Quaternion<T> Exponential(void) const {
            const Matrix<T> v = GetVector();
            const T len = v.GetLength();
            return Quaternion<T>(Cos<T>(len), v.Normalize() * Sin(len)) * Exp(a);
        }
        /// @brief q^n = exp(ln(q) * n)
        /// @param n Exponent
        /// @return Power of quaternion
        [[nodiscard]] Expected<Quaternion<T>> Pow(const T& n) const {
            if (n < 0) {
                const Expected<Quaternion<T>> tmp = GetInverse();
                return tmp.HasValue() ? tmp.Get().Pow(-n) : Expected<Quaternion<T>>();
            }
            const Expected<Quaternion<T>> tmp = Log();
            return tmp.HasValue() ? Expected<Quaternion<T>>((tmp.Get() * n).Exponential()) : Expected<Quaternion<T>>();
        }
        /// @brief q^n = exp(ln(q) * n)
        /// @param n Exponent quaternion
        /// @return Power of quaternion
        [[nodiscard]] Quaternion<T> Pow(const Quaternion<T>& n) const {
            const Expected<Quaternion<T>> tmp = Log();
            return tmp.HasValue() ? Expected<Quaternion<T>>((tmp.Get() * n).Exponential()) : Expected<Quaternion<T>>();
        }
        /// @brief q^-1 = (a - bi - cj - dk) / |q|^2
        /// @return Inverse of quaternion
        [[nodiscard]] Expected<Quaternion<T>> GetInverse(void) const {
            const T tmp = GetNormSquared();
            return FloatsEqual<T>(tmp, 0) ? Expected<Quaternion<T>>() : Expected<Quaternion<T>>(Quaternion<T>(a, -b, -c, -d) / tmp);
        }

        private:
        /// @brief (a + bi + cj + dk) + (e + fi + gj + hk) = (a + e) + (b + f)i + (c + g)j + (d + h)k
        /// @param other Other quaternion
        void Add(const Quaternion<T>& other) {
            a += other.a;
            b += other.b;
            c += other.c;
            d += other.d;
        }
        /// @brief (a + bi + cj + dk) * e = ae + (be)i + (ce)j + (de)k
        /// @param scalar Scalar value to multiply by
        void Multiply(const T& scalar) {
            a *= scalar;
            b *= scalar;
            c *= scalar;
            d *= scalar;
        }
        /// @brief a
        T a;
        /// @brief b
        T b;
        /// @brief c
        T c;
        /// @brief d
        T d;
    };
    /// @brief P' = RPR'
    /// @tparam T Type of number
    /// @param point 3D vector to rotate
    /// @param origin 3D vector to rotate around
    /// @param rotation Vector containing axis to rotate around
    /// @return Rotated 3D vector
    template <typename T>
    [[nodiscard]] Matrix<T> RotateVector(const Matrix<T>& point, const Matrix<T>& origin, const Matrix<T>& rotation) {
        const T angle = rotation.GetLength();
        if (FloatsEqual<T>(angle, 0)) return point;
        const Quaternion<T> quaternion = Quaternion<T>(Cos<T>(angle / 2), rotation.Normalize() * Sin(angle / 2));
        return (quaternion * Quaternion<T>(0, point - origin) * Quaternion<T>(quaternion.GetScalar(), -quaternion.GetVector())).GetVector() + origin;
    }
    /// @brief a x b = (a_2 * b_3 - a_3 * b_2)i + (a_3 * b_1 - a_1 * b_3)j + (a_1 * b_2 - a_2 * b_1)k
    /// @tparam T Type of number
    /// @param a A
    /// @param b B
    /// @return Cross product of vector A and B
    template <typename T>
    [[nodiscard]] Matrix<T> CrossProduct(const Matrix<T>& a, const Matrix<T>& b) {
        return (Quaternion<T>(0, a) * Quaternion<T>(0, b)).GetVector();
    }
}

#endif