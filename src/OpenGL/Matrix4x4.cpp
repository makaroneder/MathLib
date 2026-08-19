#include "Matrix4x4.hpp"
#include <Math/Trigonometry.hpp>
#include <Memory.hpp>

Matrix4x4 identity4x4 = Matrix4x4(
    Vector4(1, 0, 0, 0),
    Vector4(0, 1, 0, 0),
    Vector4(0, 0, 1, 0),
    Vector4(0, 0, 0, 1)
);
Matrix4x4 Scale(float x, float y, float z) {
    return Matrix4x4(
        Vector4(x, 0, 0, 0),
        Vector4(0, y, 0, 0),
        Vector4(0, 0, z, 0),
        Vector4(0, 0, 0, 1)
    );
}
Matrix4x4 Transalate(float x, float y, float z) {
    return Matrix4x4(
        Vector4(1, 0, 0, x),
        Vector4(0, 1, 0, y),
        Vector4(0, 0, 1, z),
        Vector4(0, 0, 0, 1)
    );
}
Matrix4x4 MakePerspectiveMatrix(float fov, float aspectRatio, float near, float far) {
    const float cot = MathLib::Cot<float>(fov / 2);
    return Matrix4x4(
        Vector4(cot / aspectRatio, 0, 0, 0),
        Vector4(0, cot, 0, 0),
        Vector4(0, 0, (near + far) / (near - far), 2 * near * far / (near - far)),
        Vector4(0, 0, -1, 0)
    );
}
Matrix4x4 RotateX(float angle) {
    const float sin = MathLib::Sin(angle);
    const float cos = MathLib::Cos<float>(angle);
    return Matrix4x4(
        Vector4(1, 0, 0, 0),
        Vector4(0, cos, -sin, 0),
        Vector4(0, sin, cos, 0),
        Vector4(0, 0, 0, 1)
    );
}
Matrix4x4 RotateY(float angle) {
    const float sin = MathLib::Sin(angle);
    const float cos = MathLib::Cos<float>(angle);
    return Matrix4x4(
        Vector4(cos, 0, sin, 0),
        Vector4(0, 1, 0, 0),
        Vector4(-sin, 0, cos, 0),
        Vector4(0, 0, 0, 1)
    );
}
Matrix4x4 RotateZ(float angle) {
    const float sin = MathLib::Sin(angle);
    const float cos = MathLib::Cos<float>(angle);
    return Matrix4x4(
        Vector4(cos, -sin, 0, 0),
        Vector4(sin, cos, 0, 0),
        Vector4(0, 0, 1, 0),
        Vector4(0, 0, 0, 1)
    );
}
Matrix4x4 Rotate(float x, float y, float z) {
    return RotateX(x) * RotateY(y) * RotateZ(z);
}
Matrix4x4::Matrix4x4(void) : data { 0, } {}
Matrix4x4::Matrix4x4(const Vector4& row0, const Vector4& row1, const Vector4& row2, const Vector4& row3) {
    MathLib::MemoryCopy(row0.data, &data[0 * width], width * sizeof(float));
    MathLib::MemoryCopy(row1.data, &data[1 * width], width * sizeof(float));
    MathLib::MemoryCopy(row2.data, &data[2 * width], width * sizeof(float));
    MathLib::MemoryCopy(row3.data, &data[3 * width], width * sizeof(float));
}
MathLib::String Matrix4x4::ToString(const MathLib::Sequence<char>& padding) const {
    const MathLib::String padd = MathLib::CollectionToString(padding);
    const MathLib::String padd2 = padd + '\t';
    MathLib::String ret = "[\n";
    for (uint8_t y = 0; y < height; y++) {
        ret += padd2;
        for (uint8_t x = 0; x < width; x++) {
            if (x) ret += ", ";
            ret += MathLib::ToString(data[y * width + x]);
        }
        ret += '\n';
    }
    return ret + padd + ']';
}
Matrix4x4 Matrix4x4::Transpose(void) const {
    Matrix4x4 ret;
    for (uint8_t y = 0; y < height; y++)
        for (uint8_t x = 0; x < width; x++) ret.data[x * width + y] = data[y * width + x];
    return ret;
}
float Matrix4x4::Trace(void) const {
    float ret = 0;
    for (uint8_t i = 0; i < width; i++) ret += data[i * width + i];
    return ret;
}
Matrix4x4& Matrix4x4::operator+=(const Matrix4x4& other) {
    for (uint8_t i = 0; i < size; i++) data[i] += other.data[i];
    return *this;
}
Matrix4x4 Matrix4x4::operator+(const Matrix4x4& other) const {
    Matrix4x4 ret = *this;
    return ret += other;
}
Matrix4x4& Matrix4x4::operator-=(const Matrix4x4& other) {
    for (uint8_t i = 0; i < size; i++) data[i] -= other.data[i];
    return *this;
}
Matrix4x4 Matrix4x4::operator-(const Matrix4x4& other) const {
    Matrix4x4 ret = *this;
    return ret -= other;
}
Matrix4x4& Matrix4x4::operator*=(const float& other) {
    for (uint8_t i = 0; i < size; i++) data[i] *= other;
    return *this;
}
Matrix4x4 Matrix4x4::operator*(const float& other) const {
    Matrix4x4 ret = *this;
    return ret *= other;
}
Matrix4x4& Matrix4x4::operator/=(const float& other) {
    for (uint8_t i = 0; i < size; i++) data[i] /= other;
    return *this;
}
Matrix4x4 Matrix4x4::operator/(const float& other) const {
    Matrix4x4 ret = *this;
    return ret /= other;
}
Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& other) {
    return *this = *this * other;
}
Matrix4x4 Matrix4x4::operator*(const Matrix4x4& other) const {
    Matrix4x4 ret;
    for (uint8_t y = 0; y < height; y++) {
        for (uint8_t x = 0; x < width; x++) {
            const uint8_t tmp = y * width;
            const uint8_t index = tmp + x;
            for (uint8_t i = 0; i < width; i++) ret.data[index] += data[tmp + i] * other.data[i * width + x];
        }
    }
    return ret;
}
Vector4 Matrix4x4::operator*(const Vector4& other) const {
    Vector4 ret;
    for (uint8_t y = 0; y < height; y++)
        for (uint8_t x = 0; x < width; x++)
            ret.data[y] += data[y * width + x] * other.data[x];
    return ret;
}
Matrix4x4 Matrix4x4::operator-(int) const {
    Matrix4x4 ret = *this;
    for (uint8_t i = 0; i < size; i++) ret.data[i] = -ret.data[i];
    return ret;
}