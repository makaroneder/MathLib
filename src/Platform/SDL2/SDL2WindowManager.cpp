#include "SDL2WindowManager.hpp"
#include "SDL2Renderer.hpp"

SDL2WindowManager::SDL2WindowManager(void) {
    if (SDL_Init(SDL_INIT_VIDEO)) MathLib::Panic(SDL_GetError());
}
SDL2WindowManager::~SDL2WindowManager(void) {
    SDL_Quit();
}
MathLib::Renderer* SDL2WindowManager::MakeRenderer(const MathLib::String& title, size_t width, size_t height) {
    return new SDL2Renderer(title, width, height);
}