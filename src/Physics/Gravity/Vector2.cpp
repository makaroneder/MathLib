#include "Vector2.hpp"

Vector2::Vector2(void) : x(0), y(0) {}
Vector2::Vector2(float x, float y) : x(x), y(y) {}
MathLib::String Vector2::ToString(const MathLib::Sequence<char>& padding) const {
    return MathLib::CollectionToString(padding) + '[' + MathLib::ToString(x) + ", " + MathLib::ToString(y) + ']';
}
float Vector2::DotProduct(const Vector2& other) const {
    return x * other.x + y * other.y;
}
float Vector2::GetLengthSquared(void) const {
    return x * x + y * y;
}
float Vector2::GetLength(void) const {
    return MathLib::Sqrt(GetLengthSquared());
}
Vector2 Vector2::operator-(void) const {
    return Vector2(-x, -y);
}
Vector2 Vector2::operator+(const Vector2& other) const {
    return Vector2(x + other.x, y + other.y);
}
Vector2 Vector2::operator-(const Vector2& other) const {
    return Vector2(x - other.x, y - other.y);
}
Vector2 Vector2::operator*(const float& scalar) const {
    return Vector2(x * scalar, y * scalar);
}
Vector2 Vector2::operator/(const float& scalar) const {
    return Vector2(x / scalar, y / scalar);
}
Vector2& Vector2::operator+=(const Vector2& other) {
    x += other.x;
    y += other.y;
    return *this;
}
Vector2& Vector2::operator-=(const Vector2& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}
Vector2& Vector2::operator*=(const float& scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}
Vector2& Vector2::operator/=(const float& scalar) {
    x /= scalar;
    y /= scalar;
    return *this;
}