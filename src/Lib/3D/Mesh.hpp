#ifndef MathLib_3D_Mesh_H
#define MathLib_3D_Mesh_H
#include "Vertex.hpp"
#include "../Renderer.hpp"

namespace MathLib {
    struct Mesh;
    struct Mesh : Comparable<Mesh> {
        Array<Array<uint32_t>> polygons;
        Array<uint32_t> triangles;
        Array<Vertex> vertices;
        Array<Image> textures;
        uint8_t polygon;

        Mesh(void);
        Mesh(uint8_t polygon, const Array<Vertex>& vertices, const Array<Image>& textures);
        Mesh(const Array<uint32_t>& triangles, const Array<Vertex>& vertices, const Array<Image>& textures);
        Mesh(const Array<Array<uint32_t>>& polygons, const Array<uint32_t>& triangles, const Array<Vertex>& vertices, const Array<Image>& textures);
        [[nodiscard]] virtual bool Equals(const Mesh& other) const override;
        void Draw(Renderer& renderer, uint32_t color) const;
    };
}

#endif