#include "ModelGenerator.hpp"

ModelGenerator::ModelGenerator(void) : firstVertex(0) {}
void ModelGenerator::Quad(size_t a, size_t b, size_t c, size_t d) {
    Face(a, b, c);
    Face(c, d, a);
}
void ModelGenerator::IsoscelesTriangle(const MathLib::Vector4& center, float size, float base) {
    const MathLib::Vector4 dx = GetX() * base / 2;
    const MathLib::Vector4 dy = GetY() * size * MathLib::Sqrt(3) / 6;
    const MathLib::Vector4 top = center + dy * 2;
    const MathLib::Vector4 down = center - dy;
    const MathLib::Vector4 left = down - dx;
    const MathLib::Vector4 right = down + dx;
    Face(Vertex(top), Vertex(left), Vertex(right));
}
void ModelGenerator::PrismWithIsoscelesTriangleBase(const MathLib::Vector4& center, float triangleSize, float triangleBase, float size) {
    const MathLib::Vector4 delta = GetZ() * size / 2;
    const size_t start = firstVertex;
    IsoscelesTriangle(center - delta, triangleSize, triangleBase);
    IsoscelesTriangle(center + delta, triangleSize, triangleBase);
    for (uint8_t i = 1; i < 3; i++) Quad(start, start + 3, start + 3 + i, start + i);
    Quad(start + 1, start + 4, start + 5, start + 2);
}