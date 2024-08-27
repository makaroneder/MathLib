#ifndef KernelRenderer_H
#define KernelRenderer_H
#include <Renderer.hpp>

/// @brief Renderer implementation
struct KernelRenderer : Renderer {
    /// @brief Creates a new renderer
    /// @param width Width of the renderer
    /// @param height Height of the renderer
    KernelRenderer(const size_t& width, const size_t& height);
    /// @brief Creates a new renderer
    /// @param width Width of the renderer
    /// @param height Height of the renderer
    /// @param framebuffer Framebuffer
    /// @param fieldPosition Bit offset of each RGB field
    KernelRenderer(const size_t& width, const size_t& height, uint32_t* framebuffer, const Color& fieldPosition);
    /// @brief Updates renderer
    /// @return Status
    virtual bool Update(void) override;
    /// @brief Return current event
    /// @return Event
    virtual Event GetEvent(void) override;
    bool AddEvent(const Event& event);

    private:
    Array<Event> events;
    uint32_t* framebuffer;
    Color fieldPosition;
};
extern KernelRenderer* renderer;

#endif