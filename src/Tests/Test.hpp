#ifndef Test_H
#define Test_H
#include <Renderer.hpp>

struct Test : Renderer {
    /// @brief Creates a new renderer
    /// @param w Width of the window
    /// @param h Height of the window
    Test(size_t w, size_t h);
    /// @brief Calculates width of the renderer window
    /// @return Width of the renderer window
    virtual size_t GetWidth(void) const override;
    /// @brief Calculates height of the renderer window
    /// @return Height of the renderer window
    virtual size_t GetHeight(void) const override;
    /// @brief Updates renderer
    /// @return Status
    virtual bool Update(void) override;
    /// @brief Renders pixel
    /// @param pixels Pixel to render
    /// @param color Color of the pixel
    /// @return Status
    virtual bool SetPixel(Matrix<num_t> pixel, uint32_t color) override;
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