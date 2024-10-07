#ifndef DummyRenderer_H
#define DummyRenderer_H
#include "Renderer.hpp"

/// @brief Renderer implementation using dummy functions
struct DummyRenderer : Renderer {
    /// @brief Creates a new renderer
    /// @param width Width of the window
    /// @param height Height of the window
    DummyRenderer(size_t width, size_t height);
    /// @brief Does nothing
    /// @return Status
    virtual bool Update(void) override;
    /// @brief Does nothing
    /// @return Event
    virtual Event GetEvent(void) override;
};

#endif