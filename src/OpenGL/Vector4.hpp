#ifndef Vector4_H
#define Vector4_H
#include <Interfaces/Printable.hpp>

struct Vector4 : MathLib::Printable {
    static constexpr uint8_t size = 4;
    float data[size];

    Vector4(void);
    Vector4(float a, float b, float c, float d);
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;
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

#endif