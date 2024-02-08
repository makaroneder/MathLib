#ifndef Test_H
#define Test_H
#include <Renderer.hpp>

struct Test : Renderer {
    /// @brief Creates a new renderer
    /// @param w Width of the window
    /// @param h Height of the window
    Test(size_t w, size_t h);
    /// @brief Flushes renderer buffer
    /// @return Status
    virtual bool Update(void) override;
    /// @brief Waits until the renderer is destroyed
    virtual void Quit(void) override;
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
    virtual bool SetPixel(Vector2<num_t> p, uint32_t color) override;
    /// @brief Draws a line
    /// @param p1 Start of the line
    /// @param p2 End of the line
    /// @param color Color of the line
    /// @return Status
    virtual bool DrawLine(Vector2<num_t> p1, Vector2<num_t> p2, uint32_t color) override;

    size_t failed;
    std::vector<Vector2<num_t>> values;
    private:
    size_t width;
    size_t height;
};

#endif