#include "SDL2Wrapper.hpp"

namespace MathLib {
    SDL2::SDL2(void) {
        if (SDL_Init(SDL_INIT_VIDEO)) Panic(SDL_GetError());
    }
    SDL2::~SDL2(void) {
        SDL_Quit();
    }
    SDL2Renderer SDL2::MakeRenderer(const String& title, size_t width, size_t height) {
        return SDL2Renderer(title, width, height);
    }
}