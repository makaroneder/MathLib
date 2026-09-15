#ifndef SDL2Renderer_H
#define SDL2Renderer_H
#define SDL_MAIN_HANDLED
#include <Renderer.hpp>
#include <SDL2/SDL.h>

struct SDL2Renderer : MathLib::Renderer {
    SDL2Renderer(const MathLib::String& title, size_t width, size_t height);
    virtual ~SDL2Renderer(void) override;
    [[nodiscard]] virtual bool Update(void) override;
    [[nodiscard]] virtual MathLib::Event GetEvent(void) override;

    private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
};

#endif