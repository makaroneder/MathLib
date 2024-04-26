#ifndef Test_H
#define Test_H
#include <Renderer.hpp>

struct Test : Renderer {
    /// @brief Creates a new renderer
    /// @param w Width of the window
    /// @param h Height of the window
    Test(size_t w, size_t h);
    /// @brief Updates renderer
    /// @param pixels Pixel to render
    /// @return Status
    virtual bool UpdateInternal(Matrix<uint32_t> pixels) override;
    /// @brief Gets current event
    /// @return Event
    virtual Event GetEvent(void) override;

    size_t failed;
    std::vector<Matrix<num_t>> values;
    private:
    size_t width;
    size_t height;
};

#endif