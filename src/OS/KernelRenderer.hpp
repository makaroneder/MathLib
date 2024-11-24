#ifndef KernelRenderer_H
#define KernelRenderer_H
#include <Renderer.hpp>

/// @brief Renderer implementation
struct KernelRenderer : MathLib::Renderer {
    /// @brief Creates a new renderer
    /// @param width Width of the renderer
    /// @param height Height of the renderer
    KernelRenderer(size_t width, size_t height);
    /// @brief Creates a new renderer
    /// @param width Width of the renderer
    /// @param height Height of the renderer
    /// @param framebuffer Framebuffer
    /// @param fieldPosition Bit offset of each RGB field
    KernelRenderer(size_t width, size_t height, uint32_t* framebuffer, const MathLib::Color& fieldPosition);
    /// @brief Updates renderer
    /// @return Status
    virtual bool Update(void) override;
    /// @brief Return current event
    /// @return Event
    virtual MathLib::Event GetEvent(void) override;
    bool AddEvent(const MathLib::Event& event);

    private:
    MathLib::Array<MathLib::Event> events;
    uint32_t* framebuffer;
    MathLib::Color fieldPosition;
};
extern KernelRenderer* renderer;

#endif