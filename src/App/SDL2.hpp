#ifndef SDL2_H
#define SDL2_H
#include "Color.hpp"
#include <Renderer.hpp>
#include <SDL2/SDL.h>

/// @brief Renderer implementation using SDL2 library
struct SDL2Renderer : Renderer {
    /// @brief Creates a new renderer
    /// @param w Width of the window
    /// @param h Height of the window
    SDL2Renderer(size_t w, size_t h);
    /// @brief Destroys the renderer
    virtual ~SDL2Renderer(void) override;
    /// @brief Flushes renderer buffer
    /// @return Status
    virtual bool Update(void) override;
    /// @brief Waits until the renderer is destroyed
    virtual void Quit(void) override;
    /// @brief Calculates width of the renderer window
    /// @return Width of the renderer window
    virtual size_t GetWidth(void) override;
    /// @brief Calculates height of the renderer window
    /// @return Height of the renderer window
    virtual size_t GetHeight(void) override;
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

    private:
    /// @brief Sets current drawing color
    /// @param c New drawing color
    /// @return Status
    bool SetColor(uint32_t c);
    /// @brief Converts graph point to internal type used to render pixels
    /// @param p Point we are converting
    void ToInternalPoint(Vector2<num_t>& p);

    /// @brief Window
    SDL_Window* window;
    /// @brief Renderer
    SDL_Renderer* renderer;
    /// @brief Width of the window
    size_t width;
    /// @brief Height of the window
    size_t height;
};

#endif