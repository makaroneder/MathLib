#ifndef SDL2Renderer_H
#define SDL2Renderer_H
#include <Renderer.hpp>
#include <SDL2/SDL.h>

namespace MathLib {
    struct SDL2;
    struct SDL2Renderer : Renderer {
        friend SDL2;
        virtual ~SDL2Renderer(void) override;
        [[nodiscard]] virtual bool Update(void) override;
        [[nodiscard]] virtual Event GetEvent(void) override;

        private:
        SDL2Renderer(const String& title, size_t width, size_t height);

        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Texture* texture;
    };
}

#endif