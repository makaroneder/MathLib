#include "DummyRenderer.hpp"

namespace MathLib {
    DummyRenderer::DummyRenderer(size_t width, size_t height) : Renderer(width, height) {}
    bool DummyRenderer::Update(void) {
        return true;
    }
    Event DummyRenderer::GetEvent(void) {
        return Event();
    }
}