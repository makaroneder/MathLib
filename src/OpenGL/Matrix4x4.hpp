#ifndef Matrix4x4_H
#define Matrix4x4_H
#include "Vector4.hpp"
#include <Math/Vector3.hpp>

struct Matrix4x4 : MathLib::Printable {
    static constexpr uint8_t width = 4;
    static constexpr uint8_t height = 4;
    static constexpr uint8_t size = width * height;
    float data[size];

    Matrix4x4(void);
    Matrix4x4(const Vector4& row0, const Vector4& row1, const Vector4& row2, const Vector4& row3);
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;
    [[nodiscard]] Matrix4x4 Transpose(void) const;
    [[nodiscard]] float Trace(void) const;
    Matrix4x4& operator+=(const Matrix4x4& other);
    [[nodiscard]] Matrix4x4 operator+(const Matrix4x4& other) const;
    Matrix4x4& operator-=(const Matrix4x4& other);
    [[nodiscard]] Matrix4x4 operator-(const Matrix4x4& other) const;
    Matrix4x4& operator*=(const float& other);
    [[nodiscard]] Matrix4x4 operator*(const float& other) const;
    Matrix4x4& operator/=(const float& other);
    [[nodiscard]] Matrix4x4 operator/(const float& other) const;
    Matrix4x4& operator*=(const Matrix4x4& other);
    [[nodiscard]] Matrix4x4 operator*(const Matrix4x4& other) const;
    [[nodiscard]] Vector4 operator*(const Vector4& other) const;
    [[nodiscard]] Matrix4x4 operator-(int) const;
};
extern Matrix4x4 identity4x4;
[[nodiscard]] Matrix4x4 Scale(float x, float y, float z);
[[nodiscard]] Matrix4x4 Transalate(float x, float y, float z);
[[nodiscard]] Matrix4x4 MakePerspectiveMatrix(float fov, float aspectRatio, float near, float far);
[[nodiscard]] Matrix4x4 RotateX(float angle);
[[nodiscard]] Matrix4x4 RotateY(float angle);
[[nodiscard]] Matrix4x4 RotateZ(float angle);
[[nodiscard]] Matrix4x4 Rotate(float x, float y, float z);

#endif