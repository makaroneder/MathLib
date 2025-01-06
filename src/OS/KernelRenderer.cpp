#include "KernelRenderer.hpp"

KernelRenderer* renderer = nullptr;
KernelRenderer::KernelRenderer(size_t width, size_t height) : KernelRenderer(width, height, nullptr, MathLib::Color(0)) {}
KernelRenderer::KernelRenderer(size_t width, size_t height, uint32_t* framebuffer, const MathLib::Color& fieldPosition) : Renderer(width, height), events(MathLib::Array<MathLib::Event>()), framebuffer(framebuffer), fieldPosition(fieldPosition) {}
bool KernelRenderer::Update(void) {
    if (framebuffer) {
        const size_t width = GetWidth();
        const size_t height = GetHeight();
        for (size_t y = 0; y < height; y++) {
            for (size_t x = 0; x < width; x++) {
                const MathLib::Color color = At(x, y);
                framebuffer[y * width + x] = (color.rgba.r << fieldPosition.rgba.r) | (color.rgba.g << fieldPosition.rgba.g) | (color.rgba.b << fieldPosition.rgba.b);
            }
        }
    }
    return true;
}
MathLib::Event KernelRenderer::GetEvent(void) {
    if (!events.GetSize()) return MathLib::Event();
    const MathLib::Event ret = events.At(0);
    MathLib::Array<MathLib::Event> newEvents = MathLib::Array<MathLib::Event>(events.GetSize() - 1);
    for (size_t i = 1; i < events.GetSize(); i++) newEvents.At(i - 1) = events.At(i);
    events = newEvents;
    return ret;
}
bool KernelRenderer::AddEvent(const MathLib::Event& event) {
    return events.Add(event);
}