#ifndef MathLib_Geometry_Model_3D_H
#define MathLib_Geometry_Model_3D_H
#include "../Triangle.hpp"

namespace MathLib {
    template <typename T>
    struct Model3D : Collection<Triangle<T>>, Printable {
        Array<Triangle<T>> triangles;

        Model3D(void) {}
        Model3D(size_t size) : triangles(size) {}
        Model3D(const Sequence<Triangle<T>>& triangles) : triangles(CollectionToArray<Triangle<T>>(triangles)) {}
        Model3D(const Sequence<Vector3<T>>& points) {
            const size_t size = points.GetSize();
            for (size_t i = 0; i < size; i += 3) {
                const Vector3<T> a = points.AtUnsafe(i);
                if (!triangles.Add(Triangle<T>(a, i + 1 < size ? points.AtUnsafe(i + 1) : a, i + 2 < size ? points.AtUnsafe(i + 2) : a))) Panic("Failed to add triangle to 3D model");
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
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override {
            const String padd = CollectionToString(padding);
            const String padd2 = padd + '\t';
            String ret = "{\n";
            for (const Triangle<T>& triangle : triangles) ret += triangle.ToString(padd2) + '\n';
            return ret + padd + '}';
        }
        [[nodiscard]] T GetMinZ(void) const {
            T z = infinity;
            for (const Triangle<T>& triangle : triangles) z = Min<T>(z, triangle.GetMinZ());
            return z;
        }
        void Draw(Renderer& renderer, uint32_t color) const {
            for (const Triangle<T>& triangle : triangles) triangle.Draw(renderer, color);
        }
        void DrawWithBackFaceCulling(Renderer& renderer, uint32_t color) const {
            for (const Triangle<T>& triangle : triangles) triangle.DrawWithBackFaceCulling(renderer, color);
        }
        [[nodiscard]] Model3D<T> operator+(const Vector3<T>& position) const {
            const size_t size = triangles.GetSize();
            Model3D<T> ret = Model3D<T>(size);
            for (size_t i = 0; i < size; i++) ret.triangles.AtUnsafe(i) = triangles.AtUnsafe(i) + position;
            return ret;
        }
        Model3D<T>& operator+=(const Vector3<T>& position) {
            for (Triangle<T>& triangle : triangles) triangle += position;
            return *this;
        }
        [[nodiscard]] Model3D<T> operator-(const Vector3<T>& position) const {
            const size_t size = triangles.GetSize();
            Model3D<T> ret = Model3D<T>(size);
            for (size_t i = 0; i < size; i++) ret.triangles.AtUnsafe(i) = triangles.AtUnsafe(i) - position;
            return ret;
        }
        Model3D<T>& operator-=(const Vector3<T>& position) {
            for (Triangle<T>& triangle : triangles) triangle -= position;
            return *this;
        }
    };
}

#endif