#include "SDL2Renderer.hpp"

namespace MathLib {
    SDL2Renderer::SDL2Renderer(const String& title, size_t width, size_t height) : Renderer(width, height) {
        window = SDL_CreateWindow(title.GetValue(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
        if (!window) Panic(SDL_GetError());
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) Panic(SDL_GetError());
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);
        if (!texture) Panic(SDL_GetError());
    }
    SDL2Renderer::~SDL2Renderer(void) {
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
    }
    bool SDL2Renderer::Update(void) {
        if (SDL_UpdateTexture(texture, nullptr, pixels.GetValue().GetValue(), GetWidth() * sizeof(uint32_t)) || SDL_RenderClear(renderer) || SDL_RenderCopy(renderer, texture, nullptr, nullptr)) return false;
        SDL_RenderPresent(renderer);
        return true;
    }
    Event SDL2Renderer::GetEvent(void) {
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT: return Event(Event::Type::Quit);
                case SDL_WINDOWEVENT: {
                    switch (event.window.type) {
                        case SDL_WINDOWEVENT_CLOSE: return Event(Event::Type::Quit);
                        default: break;
                    }
                    break;
                }
                case SDL_KEYDOWN:
                case SDL_KEYUP: return Event(event.key.keysym.sym, event.type == SDL_KEYDOWN);
                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP: return Event(CreateVector<size_t>(event.button.x, event.button.y, 0), (Event::MouseButton)event.button.button, event.type == SDL_MOUSEBUTTONDOWN);
                case SDL_MOUSEMOTION: return Event(CreateVector<size_t>(event.motion.x, event.motion.y, 0));
            }
        }
        return Event();
    }
}