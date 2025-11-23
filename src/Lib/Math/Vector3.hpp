#ifndef Vector3_H
#define Vector3_H
#include "../SingleTypePair.hpp"
#include "Vector.hpp"

namespace MathLib {
    template <typename T>
    struct Vector3 : Collection<T>, Printable {
        T x;
        T y;
        T z;

        Vector3(void) : x(0), y(0), z(0) {}
        Vector3(const T& x, const T& y, const T& z) : x(x), y(y), z(z) {}
        Vector3(const Matrix<T>& matrix) : x(GetX(matrix)), y(GetY(matrix)), z(GetZ(matrix)) {}
        [[nodiscard]] virtual size_t GetSize(void) const override {
            return 3;
        }
        [[nodiscard]] virtual bool Add(const T&) override {
            return false;
        }
        [[nodiscard]] virtual bool Reset(void) override {
            return false;
        }
        [[nodiscard]] virtual T* GetValue(void) override {
            return &x;
        }
        [[nodiscard]] virtual const T* GetValue(void) const override {
            return &x;
        }
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override {
            return CollectionToString(padding) + '[' + Formatter<T>::ToString(x) + ", " + Formatter<T>::ToString(y) + ", " + Formatter<T>::ToString(z) + ']';
        }
        [[nodiscard]] SingleTypePair<T> Project(void) const {
            T div = Abs(z);
            if (div < eps) div = eps;
            return SingleTypePair<T>(x / div, y / div);
        }
        [[nodiscard]] T GetLengthSquared(void) const {
            return x * x + y * y + z * z;
        }
        [[nodiscard]] T GetLength(void) const {
            return Sqrt(GetLengthSquared());
        }
        void Normalize(void) {
            const num_t length = GetLength();
            if (FloatsEqual<num_t>(length, 0)) return;
            x /= length;
            y /= length;
            z /= length;
        }
        [[nodiscard]] T DotProduct(const Vector3& other) const {
            return x * other.x + y * other.y + z * other.z;
        }
        [[nodiscard]] Vector3 CrossProduct(const Vector3& other) const {
            return Vector3(
                y * other.z - z * other.y,
                z * other.x - x * other.z,
                x * other.y - y * other.x
            );
        }
        [[nodiscard]] Matrix<T> ToMatrix(void) const  {
            return CreateVector<T>(x, y, z);
        }
        Vector3& operator+=(const Vector3& other) {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }
        [[nodiscard]] Vector3 operator+(const Vector3& other) const {
            return Vector3(
                x + other.x,
                y + other.y,
                z + other.z
            );
        }
        Vector3& operator-=(const Vector3& other) {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        }
        [[nodiscard]] Vector3 operator-(const Vector3& other) const {
            return Vector3(
                x - other.x,
                y - other.y,
                z - other.z
            );
        }
        Vector3& operator*=(const T& other) {
            x *= other;
            y *= other;
            z *= other;
            return *this;
        }
        [[nodiscard]] Vector3 operator*(const T& other) const {
            return Vector3(
                x * other,
                y * other,
                z * other
            );
        }
        Vector3& operator/=(const T& other) {
            x /= other;
            y /= other;
            z /= other;
            return *this;
        }
        [[nodiscard]] Vector3 operator/(const T& other) const {
            return Vector3(
                x / other,
                y / other,
                z / other
            );
        }
    };
    using vector3_t = Vector3<num_t>;
}

#endif