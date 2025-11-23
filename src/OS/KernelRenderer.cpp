#include "KernelRenderer.hpp"
#include "Arch/Arch.hpp"

KernelRenderer* renderer = nullptr;
KernelRenderer::KernelRenderer(size_t width, size_t height) : KernelRenderer(width, height, nullptr, MathLib::Color(0)) {}
KernelRenderer::KernelRenderer(size_t width, size_t height, uint32_t* framebuffer, const MathLib::Color& fieldPosition) : Renderer(width, height), events(), framebuffer(framebuffer), fieldPosition(fieldPosition) {
    for (uint8_t i = 0; i < 32; i += 8) {
        if (fieldPosition.rgba.r == i || fieldPosition.rgba.g == i || fieldPosition.rgba.b == i) continue;
        alphaPosition = this->fieldPosition.rgba.a = i;
        break;
    }
}
bool KernelRenderer::Update(void) {
    if (!framebuffer) return true;
    MathLib::MemoryCopy(pixels.GetPointer(), framebuffer, GetWidth() * GetHeight() * sizeof(uint32_t));
    return true;
}
MathLib::Event KernelRenderer::GetEvent(void) {
    if (!events.GetSize()) return MathLib::Event();
    ArchSetInterrupts(false);
    const MathLib::Event ret = events.At(0);
    const size_t size = events.GetSize();
    MathLib::Array<MathLib::Event> newEvents = MathLib::Array<MathLib::Event>(size - 1);
    for (size_t i = 1; i < size; i++) newEvents.At(i - 1) = events.At(i);
    events = newEvents;
    ArchSetInterrupts(true);
    return ret;
}
bool KernelRenderer::AddEvent(const MathLib::Event& event) {
    return events.Add(event);
}
uint32_t KernelRenderer::MapColor(uint32_t c) const {
    const MathLib::Color color = c;
    return (color.rgba.r << fieldPosition.rgba.r) | (color.rgba.g << fieldPosition.rgba.g) | (color.rgba.b << fieldPosition.rgba.b) | (color.rgba.a << fieldPosition.rgba.a);
}