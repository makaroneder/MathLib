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
    /// @brief Renders pixel
    /// @param p Position of pixel
    /// @param color Color of pixel
    /// @return Status
    virtual bool SetPixel(Vector3<num_t> p, uint32_t color) override;
    /// @brief Draws a line
    /// @param line Line to draw
    /// @param color Color of the line
    /// @return Status
    virtual bool DrawLine(Line<num_t> line, uint32_t color) override;

    size_t failed;
    std::vector<Vector3<num_t>> values;
    private:
    size_t width;
    size_t height;
};

#endif