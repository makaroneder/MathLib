#ifndef Model3D_H
#define Model3D_H
#include "Triangle.hpp"

template <typename T>
struct Model3D : MathLib::Collection<Triangle<T>>, MathLib::Printable {
    MathLib::Array<Triangle<T>> triangles;

    Model3D(void) {}
    Model3D(size_t size) : triangles(size) {}
    Model3D(const MathLib::Sequence<Triangle<T>>& triangles) : triangles(MathLib::CollectionToArray<Triangle<T>>(triangles)) {}
    Model3D(const MathLib::Sequence<MathLib::Vector3<T>>& points) {
        const size_t size = points.GetSize();
        for (size_t i = 0; i < size; i += 3) {
            const MathLib::Vector3<T> a = points.At(i);
            if (!triangles.Add(Triangle<T>(a, i + 1 < size ? points.At(i + 1) : a, i + 2 < size ? points.At(i + 2) : a))) MathLib::Panic("Failed to add triangle to 3D model");
        }
    }
    [[nodiscard]] virtual size_t GetSize(void) const override {
        return triangles.GetSize();
    }
    [[nodiscard]] virtual bool Add(const Triangle<T>& triangle) override {
        return triangles.Add(triangle);
    }
    [[nodiscard]] virtual bool Reset(void) override {
        return triangles.Reset();
    }
    [[nodiscard]] virtual Triangle<T>* GetValue(void) override {
        return triangles.GetValue();
    }
    [[nodiscard]] virtual const Triangle<T>* GetValue(void) const override {
        return triangles.GetValue();
    }
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override {
        const MathLib::String padd = MathLib::CollectionToString(padding);
        const MathLib::String padd2 = padd + '\t';
        MathLib::String ret = "{\n";
        for (const Triangle<T>& triangle : triangles) ret += triangle.ToString(padd2) + '\n';
        return ret + padd + '}';
    }
    [[nodiscard]] T GetMinZ(void) const {
        T z = MathLib::infinity;
        for (const Triangle<T>& triangle : triangles) z = MathLib::Min<T>(z, triangle.GetMinZ());
        return z;
    }
    void Draw(MathLib::Renderer& renderer, uint32_t color) const {
        for (const Triangle<T>& triangle : triangles) triangle.Draw(renderer, color);
    }
    void DrawWithBackFaceCulling(MathLib::Renderer& renderer, uint32_t color) const {
        for (const Triangle<T>& triangle : triangles) triangle.DrawWithBackFaceCulling(renderer, color);
    }
    [[nodiscard]] Model3D<T> operator+(const MathLib::Vector3<T>& position) const {
        const size_t size = triangles.GetSize();
        Model3D<T> ret = Model3D<T>(size);
        for (size_t i = 0; i < size; i++) ret.triangles.At(i) = triangles.At(i) + position;
        return ret;
    }
    Model3D<T>& operator+=(const MathLib::Vector3<T>& position) {
        for (Triangle<T>& triangle : triangles) triangle += position;
        return *this;
    }
    [[nodiscard]] Model3D<T> operator-(const MathLib::Vector3<T>& position) const {
        const size_t size = triangles.GetSize();
        Model3D<T> ret = Model3D<T>(size);
        for (size_t i = 0; i < size; i++) ret.triangles.At(i) = triangles.At(i) - position;
        return ret;
    }
    Model3D<T>& operator-=(const MathLib::Vector3<T>& position) {
        for (Triangle<T>& triangle : triangles) triangle -= position;
        return *this;
    }
};

#endif