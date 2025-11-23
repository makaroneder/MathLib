#ifndef Triangle_H
#define Triangle_H
#include <Renderer.hpp>
#include <Math/Vector3.hpp>
#include <Math/LinearInterpolation.hpp>
#include <Interfaces/Sequence/FixedSizeCollection.hpp>

template <typename T>
struct Triangle;
template <typename T>
struct Triangle : MathLib::Comparable<Triangle<T>>, MathLib::Printable {
    MathLib::Vector3<T> vertices[3];

    Triangle(void) {}
    Triangle(const MathLib::Vector3<T>& a, const MathLib::Vector3<T>& b, const MathLib::Vector3<T>& c) {
        vertices[0] = a;
        vertices[1] = b;
        vertices[2] = c;
    }
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override {
        MathLib::String ret = MathLib::CollectionToString(padding) + '[';
        for (uint8_t i = 0; i < SizeOfArray(vertices); i++) {
            if (i) ret += ", ";
            ret += vertices[i].ToString();
        }
        return ret + ']';
    }
    [[nodiscard]] T GetMinZ(void) const {
        return MathLib::Min<T>(MathLib::Min<T>(vertices[0].z, vertices[1].z), vertices[2].z);
    }
    [[nodiscard]] MathLib::Vector3<T> GetNormal(void) const {
        return (vertices[2] - vertices[0]).CrossProduct(vertices[1] - vertices[0]);
    }
    [[nodiscard]] MathLib::Vector3<T> ToBarycentric(const MathLib::Vector3<T>& point) const {
        const MathLib::Vector3<T> v0 = vertices[1] - vertices[0];
        const MathLib::Vector3<T> v1 = vertices[2] - vertices[0];
        const MathLib::Vector3<T> v2 = point - vertices[0];
        const T d00 = v0.DotProduct(v0);
        const T d01 = v0.DotProduct(v1);
        const T d11 = v1.DotProduct(v1);
        const T d20 = v2.DotProduct(v0);
        const T d21 = v2.DotProduct(v1);
        const T denom = d00 * d11 - d01 * d01;
        const T l2 = (d11 * d20 - d01 * d21) / denom;
        const T l3 = (d00 * d21 - d01 * d20) / denom;
        return MathLib::Vector3<T>(1 - l2 - l3, l2, l3);
    }
    void Draw(MathLib::Renderer& renderer, uint32_t color) const {
        if (GetZ(renderer.position) > GetMinZ()) return;
        const MathLib::Matrix<T> v1 = vertices[0].ToMatrix();
        const MathLib::Matrix<T> v2 = vertices[1].ToMatrix();
        const MathLib::Matrix<T> v3 = vertices[2].ToMatrix();
        renderer.DrawLine<T>(MathLib::Line<T>(v1, v2), color);
        renderer.DrawLine<T>(MathLib::Line<T>(v2, v3), color);
        renderer.DrawLine<T>(MathLib::Line<T>(v3, v1), color);
    }
    void DrawWithBackFaceCulling(MathLib::Renderer& renderer, uint32_t color) const {
        if ((vertices[0] - MathLib::Vector3<T>(renderer.position)).DotProduct(GetNormal()) < 0) Draw(renderer, color);
    }
    [[nodiscard]] Triangle<T> operator+(const MathLib::Vector3<T>& position) const {
        Triangle<T> ret;
        for (uint8_t i = 0; i < SizeOfArray(vertices); i++) ret.vertices[i] = vertices[i] + position;
        return ret;
    }
    Triangle<T>& operator+=(const MathLib::Vector3<T>& position) {
        for (uint8_t i = 0; i < SizeOfArray(vertices); i++) vertices[i] += position;
        return *this;
    }
    [[nodiscard]] Triangle<T> operator-(const MathLib::Vector3<T>& position) const {
        Triangle<T> ret;
        for (uint8_t i = 0; i < SizeOfArray(vertices); i++) ret.vertices[i] = vertices[i] - position;
        return ret;
    }
    Triangle<T>& operator-=(const MathLib::Vector3<T>& position) {
        for (uint8_t i = 0; i < SizeOfArray(vertices); i++) vertices[i] -= position;
        return *this;
    }

    protected:
    [[nodiscard]] virtual bool Equals(const Triangle<T>& other) const override {
        for (uint8_t i = 0; i < SizeOfArray(vertices); i++)
            if (vertices[i] != other.vertices[i]) return false;
        return true;
    }
};

#endif