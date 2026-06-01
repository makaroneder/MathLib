#ifndef MathLib_DummyRenderer_H
#define MathLib_DummyRenderer_H
#include "Renderer.hpp"

namespace MathLib {
    struct DummyRenderer : Renderer {
        DummyRenderer(size_t width, size_t height);
        [[nodiscard]] virtual bool Update(void) override;
        [[nodiscard]] virtual Event GetEvent(void) override;
    };
}

#endif