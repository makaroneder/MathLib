#include "DummyRenderer.hpp"

DummyRenderer::DummyRenderer(size_t w, size_t h) : Renderer(w, h) { }
bool DummyRenderer::Update(void) {
    return true;
}
Event DummyRenderer::GetEvent(void) {
    return Event();
}