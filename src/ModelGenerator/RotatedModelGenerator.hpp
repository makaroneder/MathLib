#ifndef RotatedModelGenerator_H
#define RotatedModelGenerator_H
#include "ModelGenerator.hpp"
#include <Math/Matrix4x4.hpp>

struct RotatedModelGenerator : ModelGenerator {
    RotatedModelGenerator(ModelGenerator& base, float x, float y, float z);
    [[nodiscard]] virtual MathLib::Vector4 GetX(void) const override;
    [[nodiscard]] virtual MathLib::Vector4 GetY(void) const override;
    [[nodiscard]] virtual MathLib::Vector4 GetZ(void) const override;
    [[nodiscard]] virtual MathLib::Vector4 GetW(void) const override;
    [[nodiscard]] virtual size_t Vertex(const MathLib::Vector4& vertex) override;
    virtual void Face(size_t a, size_t b, size_t c) override;

    private:
    ModelGenerator& base;
    MathLib::Matrix4x4 rotation;
};

#endif