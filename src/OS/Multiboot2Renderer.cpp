#include "Multiboot2Renderer.hpp"

Multiboot2Renderer::Multiboot2Renderer(Multiboot2TagFramebuffer* framebuffer) : Renderer(framebuffer->width, framebuffer->height), framebuffer(framebuffer) {}
bool Multiboot2Renderer::Update(void) {
    uint32_t* dst = (uint32_t*)framebuffer->addr;
    for (size_t y = 0; y < framebuffer->height; y++) {
        for (size_t x = 0; x < framebuffer->width; x++) {
            const Color color = pixels.At(x, y);
            dst[y * framebuffer->width + x] = (color.rgba.r << framebuffer->redFieldPos) | (color.rgba.g << framebuffer->greenFieldPos) | (color.rgba.b << framebuffer->blueFieldPos);
        }
    }
    return true;
}
Event Multiboot2Renderer::GetEvent(void) {
    return Event();
}