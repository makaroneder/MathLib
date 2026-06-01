#ifndef KernelRenderer_H
#define KernelRenderer_H
#include <Renderer.hpp>

struct KernelRenderer : MathLib::Renderer {
    KernelRenderer(size_t width, size_t height);
    KernelRenderer(size_t width, size_t height, uint32_t* framebuffer, const MathLib::Color& fieldPosition);
    [[nodiscard]] virtual bool Update(void) override;
    [[nodiscard]] virtual MathLib::Event GetEvent(void) override;
    [[nodiscard]] bool AddEvent(const MathLib::Event& event);
    [[nodiscard]] uint32_t MapColor(uint32_t color) const;

    private:
    MathLib::Array<MathLib::Event> events;
    uint32_t* framebuffer;
    MathLib::Color fieldPosition;
};
extern KernelRenderer* renderer;

#endif