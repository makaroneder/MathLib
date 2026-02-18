#ifndef Vector2_H
#define Vector2_H
#include <Interfaces/Printable.hpp>

struct Vector2 : MathLib::Printable {
    float x;
    float y;

    Vector2(void);
    Vector2(float x, float y);
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;
    [[nodiscard]] float DotProduct(const Vector2& other) const;
    [[nodiscard]] float GetLengthSquared(void) const;
    [[nodiscard]] float GetLength(void) const;
    [[nodiscard]] Vector2 operator-(void) const;
    [[nodiscard]] Vector2 operator+(const Vector2& other) const;
    [[nodiscard]] Vector2 operator-(const Vector2& other) const;
    [[nodiscard]] Vector2 operator*(const float& scalar) const;
    [[nodiscard]] Vector2 operator/(const float& scalar) const;
    Vector2& operator+=(const Vector2& other);
    Vector2& operator-=(const Vector2& other);
    Vector2& operator*=(const float& scalar);
    Vector2& operator/=(const float& scalar);
};

#endif