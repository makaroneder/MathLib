#ifndef ModelGenerator_H
#define ModelGenerator_H
#include <Math/Vector4.hpp>

struct ModelGenerator {
    size_t firstVertex;

    ModelGenerator(void);
    [[nodiscard]] virtual MathLib::Vector4 GetX(void) const = 0;
    [[nodiscard]] virtual MathLib::Vector4 GetY(void) const = 0;
    [[nodiscard]] virtual MathLib::Vector4 GetZ(void) const = 0;
    [[nodiscard]] virtual MathLib::Vector4 GetW(void) const = 0;
    [[nodiscard]] virtual size_t Vertex(const MathLib::Vector4& vertex) = 0;
    virtual void Face(size_t a, size_t b, size_t c) = 0;
    void Quad(size_t a, size_t b, size_t c, size_t d);
    void IsoscelesTriangle(const MathLib::Vector4& center, float size, float base);
    void PrismWithIsoscelesTriangleBase(const MathLib::Vector4& center, float triangleSize, float triangleBase, float size);
};

#endif