#include "ModelGenerator.hpp"
#include <Math/Matrix4x4.hpp>

ModelGenerator::ModelGenerator(MathLib::WritableSequence<MathLib::Vector4>& vertices, MathLib::WritableSequence<size_t>& faces, const MathLib::Vector4& dx, const MathLib::Vector4& dy, const MathLib::Vector4& dz, const MathLib::Vector4& dw) : vertices(vertices), faces(faces), dx(dx), dy(dy), dz(dz), dw(dw) {}
ModelGenerator ModelGenerator::Rotate(float x, float y, float z) const {
    const MathLib::Matrix4x4 rotation = MathLib::Rotate(x, y, z);
    return ModelGenerator(vertices, faces, rotation * dx, rotation * dy, rotation * dz, rotation * dw);
}
size_t ModelGenerator::Vertex(const MathLib::Vector4& vertex) {
    return vertices.Add(vertex) ? vertices.GetSize() - 1 : SIZE_MAX;
}
bool ModelGenerator::Face(size_t a, size_t b, size_t c) {
    return a != SIZE_MAX && b != SIZE_MAX && c != SIZE_MAX && faces.Add(a) && faces.Add(b) && faces.Add(c);
}
bool ModelGenerator::Quad(size_t a, size_t b, size_t c, size_t d) {
    return Face(a, b, c) && Face(c, d, a);
}
bool ModelGenerator::IsoscelesTriangle(const MathLib::Vector4& center, float size, float base) {
    base /= 2;
    const float heightBy3 = size * MathLib::Sqrt(3) / 6;
    const MathLib::Vector4 top = center + dy * heightBy3 * 2;
    const MathLib::Vector4 down = center - dy * heightBy3;
    const MathLib::Vector4 left = down - dx * base;
    const MathLib::Vector4 right = down + dx * base;
    return Face(Vertex(top), Vertex(left), Vertex(right));
}
bool ModelGenerator::PrismWithIsoscelesTriangleBase(const MathLib::Vector4& center, float triangleSize, float triangleBase, float size) {
    size /= 2;
    const size_t start = vertices.GetSize();
    if (!IsoscelesTriangle(center - dz * size, triangleSize, triangleBase)) return false;
    if (!IsoscelesTriangle(center + dz * size, triangleSize, triangleBase)) return false;
    for (uint8_t i = 1; i < 3; i++)
        if (!Quad(start, start + 3, start + 3 + i, start + i)) return false;
    return Quad(start + 1, start + 4, start + 5, start + 2);
}