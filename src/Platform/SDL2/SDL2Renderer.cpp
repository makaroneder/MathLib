#include "SDL2Renderer.hpp"

SDL2Renderer::SDL2Renderer(const MathLib::String& title, size_t width, size_t height) : MathLib::Renderer(width, height) {
    window = SDL_CreateWindow(title.GetValue(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (!window) MathLib::Panic(SDL_GetError());
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) MathLib::Panic(SDL_GetError());
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    if (!texture) MathLib::Panic(SDL_GetError());
}
SDL2Renderer::~SDL2Renderer(void) {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}
bool SDL2Renderer::Update(void) {
    if (SDL_UpdateTexture(texture, nullptr, pixels.GetPointer(), GetWidth() * sizeof(uint32_t)) || SDL_RenderClear(renderer) || SDL_RenderCopy(renderer, texture, nullptr, nullptr)) return false;
    SDL_RenderPresent(renderer);
    return true;
}
MathLib::Event SDL2Renderer::GetEvent(void) {
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT: return MathLib::Event(MathLib::Event::Type::Quit);
            case SDL_WINDOWEVENT: {
                switch (event.window.type) {
                    case SDL_WINDOWEVENT_CLOSE: return MathLib::Event(MathLib::Event::Type::Quit);
                    default: break;
                }
                break;
            }
            case SDL_KEYDOWN:
            case SDL_KEYUP: return MathLib::Event(event.key.keysym.sym, event.type == SDL_KEYDOWN);
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP: return MathLib::Event(event.button.x, event.button.y, (MathLib::Event::MouseButton)event.button.button, event.type == SDL_MOUSEBUTTONDOWN);
            case SDL_MOUSEMOTION: return MathLib::Event((size_t)event.motion.x, (size_t)event.motion.y);
        }
    }
    return MathLib::Event();
}