#ifndef MathLib_DummyRenderer_H
#define MathLib_DummyRenderer_H
#include "Renderer.hpp"

namespace MathLib {
    /// @brief Renderer implementation using dummy functions
    struct DummyRenderer : Renderer {
        /// @brief Creates a new renderer
        /// @param width Width of the window
        /// @param height Height of the window
        DummyRenderer(size_t width, size_t height);
        /// @brief Does nothing
        /// @return Status
        [[nodiscard]] virtual bool Update(void) override;
        /// @brief Does nothing
        /// @return Event
        [[nodiscard]] virtual Event GetEvent(void) override;
    };
    #define LoadImage(name, path_)                                  \
        MathLib::DummyRenderer name = MathLib::DummyRenderer(0, 0); \
        if (!name.SetImage<MathLib::TGA>() || !name.LoadFromPath(fs, path + path_)) MathLib::Panic("Failed to load image")
}

#endif