#ifndef SDL2_H
#define SDL2_H
#include <Renderer.hpp>
#include <SDL2/SDL.h>

/// @brief Renderer implementation using SDL2 library
struct SDL2Renderer : Renderer {
    /// @brief Creates a new renderer
    /// @param title Title of the window
    /// @param w Width of the window
    /// @param h Height of the window
    SDL2Renderer(const char* title, size_t w, size_t h);
    /// @brief Destroys the renderer
    virtual ~SDL2Renderer(void) override;
    /// @brief Calculates width of the renderer window
    /// @return Width of the renderer window
    virtual size_t GetWidth(void) const override;
    /// @brief Calculates height of the renderer window
    /// @return Height of the renderer window
    virtual size_t GetHeight(void) const override;
    /// @brief Flushes renderer buffer
    /// @return Status
    virtual bool Update(void) override;
    /// @brief Renders pixel
    /// @param pixels Pixel to render
    /// @param color Color of the pixel
    /// @return Status
    virtual bool SetPixel(Matrix<num_t> pixel, uint32_t color) override;
    /// @brief Gets current SDL2 event
    /// @return SDL2 event
    virtual Event GetEvent(void) override;

    private:
    /// @brief Sets current drawing color
    /// @param c New drawing color
    /// @return Status
    bool SetColor(uint32_t c);
    /// @brief Converts graph point to internal type used to render pixels
    /// @param p Point we are converting
    /// @return Is point visable?
    bool ToInternalPoint(Matrix<num_t>& p);

    /// @brief Window
    SDL_Window* window;
    /// @brief Renderer
    SDL_Renderer* renderer;
    /// @brief Width of the window
    size_t width;
    /// @brief Height of the window
    size_t height;
    uint32_t* pixels;
};

#endif