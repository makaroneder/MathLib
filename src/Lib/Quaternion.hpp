#ifndef Quaternion_H
#define Quaternion_H
#include "Vector.hpp"

/// @brief q = a + bi + cj + dk
/// @tparam T Type of number
template <typename T>
struct Quaternion : Printable {
    CreateOperators(Quaternion<T>, T)
    /// @brief q = a + bi + cj + dk
    /// @param a_ a
    /// @param b_ b
    /// @param c_ c
    /// @param d_ d
    Quaternion(T a_, T b_, T c_, T d_) : a(a_), b(b_), c(c_), d(d_) {}
    /// @brief q = a + v_0i + v_1j + v_2k
    /// @param scalar Scalar part of quaternion
    /// @param vector Vector part of quaternion
    Quaternion(T scalar, Matrix<T> vector) : a(scalar), b(GetX(vector)), c(GetY(vector)), d(GetZ(vector)) {}
    /// @brief Returns scalar part of quaternion
    /// @return Scalar part of quaternion
    constexpr T GetScalar(void) const {
        return a;
    }
    /// @brief Returns vector part of quaternion
    /// @return Vector part of quaternion
    constexpr Matrix<T> GetVector(void) const {
        return CreateVector<T>(b, c, d);
    }
    /// @brief Converts quaternion to string
    /// @param padding String to pad with
    /// @return String representation
    virtual std::string ToString(std::string padding = "") const override {
        std::string ret = FloatsEqual<T>(a, 0) ? "" : std::to_string(a);
        if (FloatsEqual<T>(b, 1)) ret += ret.empty() ? "i" : " + i";
        else if (!FloatsEqual<T>(b, 0)) ret += (ret.empty() ? "" : " + ") + std::to_string(b) + 'i';
        if (FloatsEqual<T>(c, 1)) ret += ret.empty() ? "j" : " + j";
        else if (!FloatsEqual<T>(c, 0)) ret += (ret.empty() ? "" : " + ") + std::to_string(c) + 'j';
        if (FloatsEqual<T>(d, 1)) ret += ret.empty() ? "k" : " + k";
        else if (!FloatsEqual<T>(d, 0)) ret += (ret.empty() ? "" : " + ") + std::to_string(d) + 'k';
        return padding + (ret.empty() ? "0" : ret);
    }
    /// @brief (a + bi + cj + dk) * (e + fi + gj + hk) = (ae - bf - cg - dh) + (af + be + ch - dg)i + (ag - bh + ce + df)j + (ah + bg - cf + de)k
    /// @param other Other quaternion
    /// @return New quaternion
    constexpr Quaternion<T> operator*(Quaternion<T> other) const {
        return Quaternion<T>(
            a * other.a - b * other.b - c * other.c - d * other.d,
            a * other.b + b * other.a + c * other.d - d * other.c,
            a * other.c - b * other.d + c * other.a + d * other.b,
            a * other.d + b * other.c - c * other.b + d * other.a
        );
    }
    /// @brief |q|^2 = a^2 + b^2 + c^2 + d^2
    /// @return Squared norm of quaternion
    T GetNormSquared(void) const {
        return std::pow(a, 2) + std::pow(b, 2) + std::pow(c, 2) + std::pow(d, 2);
    }
    /// @brief |q| = sqrt(a^2 + b^2 + c^2 + d^2)
    /// @return Norm of quaternion
    T GetNorm(void) const {
        return std::sqrt(GetNormSquared());
    }
    /// @brief ln(q) = ln(|q|) + v / |v| * arccos(a / |q|)
    /// @return Logarithm of quaternion
    Quaternion<T> Log(void) const {
        const Matrix<T> v = GetVector();
        const T norm = GetNorm();
        return Quaternion<T>(std::log(norm), v / v.GetLength() * std::acos(a / norm));
    }
    /// @brief e^q = e^a * (cos(|v|) + v / |v| * sin(|v|))
    /// @return Exponential of quaternion
    Quaternion<T> Exponential(void) const {
        const Matrix<T> v = GetVector();
        const T len = v.GetLength();
        return Quaternion<T>(std::cos(len), v / len * std::sin(len)) * std::exp(a);
    }
    /// @brief q^n = exp(ln(q) * n)
    /// @param n Exponent
    /// @return Power of quaternion
    Quaternion<T> Pow(T n) const {
        if (n < 0) return GetInverse().Pow(std::abs(n));
        return (Log() * n).Exponential();
    }
    /// @brief q^n = exp(ln(q) * n)
    /// @param n Exponent quaternion
    /// @return Power of quaternion
    Quaternion<T> Pow(Quaternion<T> n) const {
        return (Log() * n).Exponential();
    }
    /// @brief q^-1 = (a - bi - cj - dk) / |q|^2
    /// @return Inverse of quaternion
    Quaternion<T> GetInverse(void) const {
        return Quaternion<T>(a, -b, -c, -d) / GetNormSquared();
    }

    private:
    /// @brief (a + bi + cj + dk) + (e + fi + gj + hk) = (a + e) + (b + f)i + (c + g)j + (d + h)k
    /// @param other Other quaternion
    void Add(Quaternion<T> other) {
        a += other.a;
        b += other.b;
        c += other.c;
        d += other.d;
    }
    /// @brief (a + bi + cj + dk) * e = ae + (be)i + (ce)j + (de)k
    /// @param scalar Scalar value to multiply by
    void Multiply(T scalar) {
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
Matrix<T> RotateVector(Matrix<T> point, Matrix<T> origin, Matrix<T> rotation) {
    const T angle = rotation.GetLength();
    const Quaternion<T> quaternion = Quaternion<T>(std::cos(angle / 2), rotation.Normalize() * std::sin(angle / 2));
    return (quaternion * Quaternion<T>(0, point - origin) * Quaternion<T>(quaternion.GetScalar(), -quaternion.GetVector())).GetVector() + origin;
}

#endif