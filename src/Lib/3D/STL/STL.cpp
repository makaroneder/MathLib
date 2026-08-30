#include "STL.hpp"

namespace MathLib {
    bool STL::Save(Writable& file) const {
        const Array<Vertex> vertices = ToTriangles();
        const size_t size = vertices.GetSize();
        if (!file.Write<STLHeader>(STLHeader(size / 3))) return false;
        for (size_t i = 0; i < size; i += 3)
            if (!file.Write<STLTriangle>(STLTriangle(vertices.AtUnsafe(i * 3 + 0).position, vertices.AtUnsafe(i * 3 + 1).position, vertices.AtUnsafe(i * 3 + 2).position))) return false;
        return true;
    }
    bool STL::Load(Readable& file) {
        STLHeader header;
        if (!file.Read<STLHeader>(header)) return false;
        Array<Vertex> vertices = header.triangleCount * 3;
        for (uint64_t i = 0; i < header.triangleCount; i++) {
            STLTriangle triangle;
            if (!file.Read<STLTriangle>(triangle)) return false;
            for (uint8_t j = 0; j < 3; j++) vertices.AtUnsafe(i * 3 + j) = Vertex(triangle.vertices[j].ToVector3(), Vector3<float>(), Vector3<float>());
        }
        meshes = MakeArray<Mesh>(Mesh(3, vertices, Array<Image>()));
        return true;
    }
}