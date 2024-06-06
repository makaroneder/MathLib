#include "SDL2.hpp"

SDL2Renderer::SDL2Renderer(const char* title, size_t w, size_t h) : Renderer(w, h) {
    if (SDL_Init(SDL_INIT_VIDEO)) Panic("Failed to initialize SDL2");
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
    if (window == nullptr) Panic("Failed to initialize window");
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) Panic("Failed to initialize renderer");
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, w, h);
}
SDL2Renderer::~SDL2Renderer(void) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(texture);
    SDL_Quit();
}
bool SDL2Renderer::Update(void) {
    if (SDL_UpdateTexture(texture, nullptr, GetPixels().GetValue().GetValue(), GetWidth() * sizeof(uint32_t))) return false;
    if (SDL_RenderClear(renderer)) return false;
    if (SDL_RenderCopy(renderer, texture, nullptr, nullptr)) return false;
    SDL_RenderPresent(renderer);
    return true;
}
Event SDL2Renderer::GetEvent(void) {
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT: return Event(Event::Type::Quit);
            case SDL_KEYDOWN: return Event(event.key.keysym.sym, true);
            case SDL_KEYUP: return Event(event.key.keysym.sym, false);
        }
    }
    return Event();
}