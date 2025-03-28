#ifndef SDL2Renderer_H
#define SDL2Renderer_H
#include <Renderer.hpp>
#include <SDL2/SDL.h>

namespace MathLib {
    struct SDL2;
    /// @brief Renderer implementation using SDL2 library
    struct SDL2Renderer : Renderer {
        friend SDL2;
        /// @brief Destroys the renderer
        virtual ~SDL2Renderer(void) override;
        /// @brief Copies pixels from memory to screen
        /// @return Status
        [[nodiscard]] virtual bool Update(void) override;
        /// @brief Gets current event
        /// @return Event
        [[nodiscard]] virtual Event GetEvent(void) override;

        private:
        /// @brief Creates a new renderer
        /// @param title Title of the window
        /// @param width Width of the window
        /// @param height Height of the window
        SDL2Renderer(const String& title, size_t width, size_t height);

        /// @brief Window
        SDL_Window* window;
        /// @brief Renderer
        SDL_Renderer* renderer;
        /// @brief Texture
        SDL_Texture* texture;
    };
}

#endif