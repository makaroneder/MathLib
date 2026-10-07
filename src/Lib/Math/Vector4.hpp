#ifndef MathLib_Math_Vector4_H
#define MathLib_Math_Vector4_H
#include "../Interfaces/Printable.hpp"

namespace MathLib {
    struct Vector4;
    struct Vector4 : Comparable<Vector4>, Printable {
        static constexpr uint8_t size = 4;
        float data[size];

        Vector4(void);
        Vector4(float a, float b, float c, float d);
        [[nodiscard]] virtual bool Equals(const Vector4& other) const override;
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override;
        Vector4& operator+=(const Vector4& other);
        [[nodiscard]] Vector4 operator+(const Vector4& other) const;
        Vector4& operator-=(const Vector4& other);
        [[nodiscard]] Vector4 operator-(const Vector4& other) const;
        Vector4& operator*=(const float& other);
        [[nodiscard]] Vector4 operator*(const float& other) const;
        Vector4& operator/=(const float& other);
        [[nodiscard]] Vector4 operator/(const float& other) const;
        [[nodiscard]] Vector4 operator-(int) const;
        [[nodiscard]] float operator*(const Vector4& other) const;
    };
}

#endif