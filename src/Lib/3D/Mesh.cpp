#include "Mesh.hpp"

namespace MathLib {
    Mesh::Mesh(void) : polygons(), triangles(), vertices(), textures(), polygon(0) {}
    Mesh::Mesh(uint8_t polygon, const Array<Vertex>& vertices, const Array<Image>& textures) : polygons(), triangles(), vertices(vertices), textures(textures), polygon(polygon) {}
    Mesh::Mesh(const Array<uint32_t>& triangles, const Array<Vertex>& vertices, const Array<Image>& textures) : polygons(), triangles(triangles), vertices(vertices), textures(textures), polygon(0) {}
    Mesh::Mesh(const Array<Array<uint32_t>>& polygons, const Array<uint32_t>& triangles, const Array<Vertex>& vertices, const Array<Image>& textures) : polygons(polygons), triangles(triangles), vertices(vertices), textures(textures), polygon(0) {}
    bool Mesh::Equals(const Mesh& other) const {
        return polygon == other.polygon && triangles == other.triangles && vertices == other.vertices && polygons == other.polygons && textures == other.textures;
    }
    void Mesh::Draw(Renderer& renderer, uint32_t color) const {
        if (!polygon) {
            const size_t triangleCount = triangles.GetSize();
            for (size_t i = 0; i < triangleCount; i += 3) {
                for (uint8_t j = 0; j < 3; j++)
                    renderer.DrawLine<float>(MathLib::Line<float>(vertices.At(triangles.At(i + j)).position.ToMatrix(), vertices.At(triangles.At(i + (j + 1) % polygon)).position.ToMatrix()), color);
            }
            for (const Sequence<uint32_t>& polygon : polygons) {
                const size_t size = polygon.GetSize();
                for (size_t i = 0; i < size; i++)
                    renderer.DrawLine<float>(MathLib::Line<float>(vertices.At(polygon.AtUnsafe(i)).position.ToMatrix(), vertices.At(polygon.AtUnsafe((i + 1) % size)).position.ToMatrix()), color);
            }
            return;
        }
        const size_t size = vertices.GetSize();
        if (polygon == 1) {
            for (size_t i = 0; i < size; i++) renderer.SetPixel<float>(vertices.AtUnsafe(i).position.ToMatrix(), color);
            return;
        }
        for (size_t i = 0; i < size; i += polygon) {
            for (uint8_t j = 0; j < polygon; j++)
                renderer.DrawLine<float>(MathLib::Line<float>(vertices.At(i + j).position.ToMatrix(), vertices.At(i + (j + 1) % polygon).position.ToMatrix()), color);
        }
    }
}