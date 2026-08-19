#include "Vector4.hpp"

Vector4::Vector4(void) : data { 0, 0, 0, 0 } {}
Vector4::Vector4(float a, float b, float c, float d) : data { a, b, c, d } {}
MathLib::String Vector4::ToString(const MathLib::Sequence<char>&) const {
    MathLib::String ret = '['_M;
    for (uint8_t i = 0; i < size; i++) {
        if (i) ret += ", ";
        ret += MathLib::ToString(data[i]);
    }
    return ret + ']';
}
Vector4& Vector4::operator+=(const Vector4& other) {
    for (uint8_t i = 0; i < size; i++) data[i] += other.data[i];
    return *this;
}
Vector4 Vector4::operator+(const Vector4& other) const {
    Vector4 ret = *this;
    return ret += other;
}
Vector4& Vector4::operator-=(const Vector4& other) {
    for (uint8_t i = 0; i < size; i++) data[i] -= other.data[i];
    return *this;
}
Vector4 Vector4::operator-(const Vector4& other) const {
    Vector4 ret = *this;
    return ret -= other;
}
Vector4& Vector4::operator*=(const float& other) {
    for (uint8_t i = 0; i < size; i++) data[i] *= other;
    return *this;
}
Vector4 Vector4::operator*(const float& other) const {
    Vector4 ret = *this;
    return ret *= other;
}
Vector4& Vector4::operator/=(const float& other) {
    for (uint8_t i = 0; i < size; i++) data[i] /= other;
    return *this;
}
Vector4 Vector4::operator/(const float& other) const {
    Vector4 ret = *this;
    return ret /= other;
}
Vector4 Vector4::operator-(int) const {
    return Vector4(-data[0], -data[1], -data[2], -data[3]);
}
float Vector4::operator*(const Vector4& other) const {
    float ret = 0;
    for (uint8_t i = 0; i < size; i++) ret += data[i] * other.data[i];
    return ret;
}