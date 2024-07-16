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
    SDL2Renderer(const String& title, const size_t& w, const size_t& h);
    /// @brief Destroys the renderer
    virtual ~SDL2Renderer(void) override;
    /// @brief Copies pixels from memory to screen
    /// @return Status
    virtual bool Update(void) override;
    /// @brief Gets current event
    /// @return Event
    virtual Event GetEvent(void) override;

    private:
    /// @brief Window
    SDL_Window* window;
    /// @brief Renderer
    SDL_Renderer* renderer;
    /// @brief Texture
    SDL_Texture* texture;
};

#endif