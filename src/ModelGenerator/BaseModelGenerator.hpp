#ifndef BaseModelGenerator_H
#define BaseModelGenerator_H
#include "ModelGenerator.hpp"

struct BaseModelGenerator : ModelGenerator {
    [[nodiscard]] virtual MathLib::Vector4 GetX(void) const override;
    [[nodiscard]] virtual MathLib::Vector4 GetY(void) const override;
    [[nodiscard]] virtual MathLib::Vector4 GetZ(void) const override;
    [[nodiscard]] virtual MathLib::Vector4 GetW(void) const override;
};

#endif