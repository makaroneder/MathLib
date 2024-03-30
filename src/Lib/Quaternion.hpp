#ifndef Quaternion_H
#define Quaternion_H
#include "Vector.hpp"

/// @brief q = a + bi + cj + dk
/// @tparam T Type of number
template <typename T>
struct Quaternion : Printable {
    /// @brief q = a + bi + cj + dk
    /// @param a_ a
    /// @param b_ b
    /// @param c_ c
    /// @param d_ d
    Quaternion(T a_, T b_, T c_, T d_) {
        a = a_;
        b = b_;
        c = c_;
        d = d_;
    }
    /// @brief q = a + v_0i + v_1j + v_2k
    /// @param scalar Scalar part of quaternion
    /// @param vector Vector part of quaternion
    Quaternion(T scalar, Matrix<T> vector) {
        a = scalar;
        b = GetX(vector);
        c = GetY(vector);
        d = GetZ(vector);
    }
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
    /// @brief (a + bi + cj + dk) + (e + fi + gj + hk) = (a + e) + (b + f)i + (c + g)j + (d + h)k
    /// @param other Other quaternion
    /// @return New quaternion
    constexpr Quaternion<T> operator+(Quaternion<T> other) const {
        return Quaternion<T>(a + other.a, b + other.b, c + other.c, d + other.d);
    }
    /// @brief (a + bi + cj + dk) - (e + fi + gj + hk) = (a - e) + (b - f)i + (c - g)j + (d - h)k
    /// @param other Other quaternion
    /// @return New quaternion
    constexpr Quaternion<T> operator-(Quaternion<T> other) const {
        return Quaternion<T>(a - other.a, b - other.b, c - other.c, d - other.d);
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
    /// @brief (a + bi + cj + dk) * e = ae + (be)i + (ce)j + (de)k
    /// @param scalar Scalar value
    /// @return New quaternion
    constexpr Quaternion<T> operator*(T scalar) const {
        return Quaternion<T>(a * scalar, b * scalar, c * scalar, d * scalar);
    }
    /// @brief (a + bi + cj + dk) / e = a / e + (b / e)i + (c / e)j + (d / e)k
    /// @param scalar Scalar value
    /// @return New quaternion
    constexpr Quaternion<T> operator/(T scalar) const {
        return Quaternion<T>(a / scalar, b / scalar, c / scalar, d / scalar);
    }

    private:
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
/// @param angle Angles to rotate by
/// @param axis Normalized vector containing axis to rotate around
/// @return Rotated 3D vector
template <typename T>
Matrix<T> RotateVector(Matrix<T> point, Matrix<T> origin, T angle, Matrix<T> axis) {
    const Quaternion<T> rotation = Quaternion<T>(std::cos(angle / 2), axis * std::sin(angle / 2));
    return (rotation * Quaternion<T>(0, point - origin) * Quaternion<T>(rotation.GetScalar(), -rotation.GetVector())).GetVector() + origin;
}

#endif