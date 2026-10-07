#ifndef ModelGenerator_H
#define ModelGenerator_H
#include <Math/Vector4.hpp>

struct ModelGenerator {
    MathLib::WritableSequence<MathLib::Vector4>& vertices;
    MathLib::WritableSequence<size_t>& faces;
    MathLib::Vector4 dx;
    MathLib::Vector4 dy;
    MathLib::Vector4 dz;
    MathLib::Vector4 dw;

    ModelGenerator(MathLib::WritableSequence<MathLib::Vector4>& vertices, MathLib::WritableSequence<size_t>& faces, const MathLib::Vector4& dx, const MathLib::Vector4& dy, const MathLib::Vector4& dz, const MathLib::Vector4& dw);
    [[nodiscard]] ModelGenerator Rotate(float x, float y, float z) const;
    [[nodiscard]] size_t Vertex(const MathLib::Vector4& vertex);
    [[nodiscard]] bool Face(size_t a, size_t b, size_t c);
    [[nodiscard]] bool Quad(size_t a, size_t b, size_t c, size_t d);
    [[nodiscard]] bool IsoscelesTriangle(const MathLib::Vector4& center, float size, float base);
    [[nodiscard]] bool PrismWithIsoscelesTriangleBase(const MathLib::Vector4& center, float triangleSize, float triangleBase, float size);
};

#endif