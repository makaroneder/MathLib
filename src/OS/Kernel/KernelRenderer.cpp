#include "KernelRenderer.hpp"
#include "Arch/Arch.hpp"

KernelRenderer* renderer = nullptr;
KernelRenderer::KernelRenderer(size_t width, size_t height) : KernelRenderer(width, height, nullptr, MathLib::Color(0)) {}
KernelRenderer::KernelRenderer(size_t width, size_t height, uint32_t* framebuffer, const MathLib::Color& fieldPosition) : Renderer(width, height), events(), framebuffer(framebuffer), fieldPosition(fieldPosition) {
    for (uint8_t i = 0; i < 32; i += 8)
        if (fieldPosition.rgba.r != i && fieldPosition.rgba.g != i && fieldPosition.rgba.b != i)
            alphaPosition = this->fieldPosition.rgba.a = i;
}
bool KernelRenderer::Update(void) {
    if (!framebuffer) return true;
    MathLib::MemoryCopy(pixels.GetPointer(), framebuffer, GetWidth() * GetHeight() * sizeof(uint32_t));
    return true;
}
MathLib::Event KernelRenderer::GetEvent(void) {
    ArchSetInterrupts(false);
    const size_t size = events.GetSize();
    if (!size) {
        ArchSetInterrupts(true);
        return MathLib::Event();
    }
    const MathLib::Event ret = events.AtUnsafe(0);
    MathLib::Array<MathLib::Event> newEvents = MathLib::Array<MathLib::Event>(size - 1);
    for (size_t i = 1; i < size; i++) newEvents.AtUnsafe(i - 1) = events.AtUnsafe(i);
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