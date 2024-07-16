#include "DummyRenderer.hpp"

DummyRenderer::DummyRenderer(const size_t& w, const size_t& h) : Renderer(w, h) { }
bool DummyRenderer::Update(void) {
    return true;
}
Event DummyRenderer::GetEvent(void) {
    return Event();
}