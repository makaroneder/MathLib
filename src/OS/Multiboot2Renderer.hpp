#ifndef Multiboot2Renderer_H
#define Multiboot2Renderer_H
#include "Multiboot2Info.hpp"
#include <Renderer.hpp>

/// @brief Renderer implementation using multiboot2 framebuffer tag
struct Multiboot2Renderer : Renderer {
    /// @brief Creates a new renderer
    /// @param framebuffer Multiboot2 framebuffer tag
    Multiboot2Renderer(Multiboot2TagFramebuffer* framebuffer);
    /// @brief Copies data to framebuffer
    /// @return Status
    virtual bool Update(void) override;
    /// @brief Does nothing
    /// @return Event
    virtual Event GetEvent(void) override;

    private:
    Multiboot2TagFramebuffer* framebuffer;
};

#endif