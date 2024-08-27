#include "KernelRenderer.hpp"

KernelRenderer* renderer = nullptr;
KernelRenderer::KernelRenderer(const size_t& width, const size_t& height) : KernelRenderer(width, height, nullptr, Color(0)) {}
KernelRenderer::KernelRenderer(const size_t& width, const size_t& height, uint32_t* framebuffer, const Color& fieldPosition) : Renderer(width, height), events(Array<Event>(0)), framebuffer(framebuffer), fieldPosition(fieldPosition) {}
bool KernelRenderer::Update(void) {
    if (framebuffer) {
        const size_t width = GetWidth();
        const size_t height = GetHeight();
        for (size_t y = 0; y < height; y++) {
            for (size_t x = 0; x < width; x++) {
                const Color color = pixels.At(x, y);
                framebuffer[y * width + x] = (color.rgba.r << fieldPosition.rgba.r) | (color.rgba.g << fieldPosition.rgba.g) | (color.rgba.b << fieldPosition.rgba.b);
            }
        }
    }
    return true;
}
Event KernelRenderer::GetEvent(void) {
    if (!events.GetSize()) return Event();
    const Event ret = events.At(0);
    Array<Event> newEvents = Array<Event>(events.GetSize() - 1);
    for (size_t i = 1; i < events.GetSize(); i++) newEvents.At(i - 1) = events.At(i);
    events = newEvents;
    return ret;
}
bool KernelRenderer::AddEvent(const Event& event) {
    return events.Add(event);
}