#ifndef WavefrontObjModelGenerator_H
#define WavefrontObjModelGenerator_H
#include "BaseModelGenerator.hpp"

struct WavefrontObjModelGenerator : BaseModelGenerator {
    [[nodiscard]] virtual size_t Vertex(const MathLib::Vector4& vertex) override;
    virtual void Face(size_t a, size_t b, size_t c) override;
};

#endif