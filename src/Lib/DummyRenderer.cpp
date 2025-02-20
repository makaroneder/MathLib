#include "DummyRenderer.hpp"

namespace MathLib {
    DummyRenderer::DummyRenderer(size_t width, size_t height) : Renderer(width, height) {
        EmptyBenchmark
    }
    bool DummyRenderer::Update(void) {
        StartAndReturnFromBenchmark(true);
    }
    Event DummyRenderer::GetEvent(void) {
        StartAndReturnFromBenchmark(Event());
    }
}