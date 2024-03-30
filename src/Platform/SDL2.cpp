#include "SDL2.hpp"
#include <iostream>

SDL2Renderer::SDL2Renderer(const char* title, size_t w, size_t h) : Renderer() {
    width = w;
    height = h;
    if (SDL_Init(SDL_INIT_VIDEO) != 0) throw std::runtime_error("Failed to initialize renderer");
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
    if (window == nullptr) throw std::runtime_error("Failed to initialize window");
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == nullptr) throw std::runtime_error("Failed to initialize renderer");
    pixels = new uint32_t[width * height];
}
SDL2Renderer::~SDL2Renderer(void) {
    delete [] pixels;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
size_t SDL2Renderer::GetWidth(void) const {
    return width;
}
size_t SDL2Renderer::GetHeight(void) const {
    return height;
}
bool SDL2Renderer::SetPixel(Matrix<num_t> pixel, uint32_t color) {
    if (!SetColor(color)) return false;
    if (!ToInternalPoint(pixel)) return true;
    return SDL_RenderDrawPointF(renderer, GetX(pixel), GetY(pixel)) == 0;
}
bool SDL2Renderer::Update(void) {
    SDL_RenderPresent(renderer);
    return SetColor(0) && SDL_RenderClear(renderer) == 0;
}
Event SDL2Renderer::GetEvent(void) {
    SDL_Event event;
    if (SDL_PollEvent(&event) != 0) {
        switch (event.type) {
            case SDL_QUIT: return Event(Event::Type::Quit);
            case SDL_KEYDOWN: return Event(event.key.keysym.sym, true);
            case SDL_KEYUP: return Event(event.key.keysym.sym, false);
        }
    }
    return Event();
}
bool SDL2Renderer::SetColor(uint32_t c) {
    Color color = Color(c);
    return SDL_SetRenderDrawColor(renderer, color.rgba.r, color.rgba.g, color.rgba.b, color.rgba.a) == 0;
}
bool SDL2Renderer::ToInternalPoint(Matrix<num_t>& p) {
    p = ConvertVectorToVector2<num_t>(p);
    if (p.IsNan()) return false;
    p *= pointMultiplier;
    if (GetY(p) < 0) GetY(p) = (height / 2) + GetY(p);
    else GetY(p) += height / 2;
    if (GetX(p) < 0) GetX(p) = (width / 2) + GetX(p);
    else GetX(p) += width / 2;
    GetY(p) = height - GetY(p);
    return true;
}