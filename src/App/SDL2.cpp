#include "SDL2.hpp"
#include <iostream>

SDL2Renderer::SDL2Renderer(size_t w, size_t h) {
    width = w;
    height = h;
    if (SDL_Init(SDL_INIT_VIDEO) != 0) throw std::runtime_error("Failed to initialize renderer");
    window = SDL_CreateWindow("Math graph", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
    if (window == nullptr) throw std::runtime_error("Failed to initialize window");
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == nullptr) throw std::runtime_error("Failed to initialize renderer");
}
SDL2Renderer::~SDL2Renderer(void) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
size_t SDL2Renderer::GetWidth(void) {
    return width;
}
size_t SDL2Renderer::GetHeight(void) {
    return height;
}
bool SDL2Renderer::SetPixel(Vector2<num_t> p, uint32_t color) {
    if (!SetColor(color)) return false;
    ToInternalPoint(p);
    return SDL_RenderDrawPointF(renderer, GetVectorAxis(p, VectorAxis::X), GetVectorAxis(p, VectorAxis::Y)) == 0;
}
bool SDL2Renderer::DrawLine(Vector2<num_t> p1, Vector2<num_t> p2, uint32_t color) {
    if (!SetColor(color)) return false;
    ToInternalPoint(p1);
    ToInternalPoint(p2);
    return SDL_RenderDrawLineF(renderer, GetVectorAxis(p1, VectorAxis::X), GetVectorAxis(p1, VectorAxis::Y), GetVectorAxis(p2, VectorAxis::X), GetVectorAxis(p2, VectorAxis::Y)) == 0;
}
bool SDL2Renderer::Update(void) {
    SDL_RenderPresent(renderer);
    return SDL_RenderClear(renderer) == 0;
}
void SDL2Renderer::Quit(void) {
    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                break;
            }
        }
    }
}
bool SDL2Renderer::SetColor(uint32_t c) {
    Color color = Color(c);
    return SDL_SetRenderDrawColor(renderer, color.rgba.r, color.rgba.g, color.rgba.b, color.rgba.a) == 0;
}
void SDL2Renderer::ToInternalPoint(Vector2<num_t>& p) {
    GetVectorAxis(p, VectorAxis::X) *= pointMultiplier;
    GetVectorAxis(p, VectorAxis::Y) *= pointMultiplier;
    if (GetVectorAxis(p, VectorAxis::Y) < 0) GetVectorAxis(p, VectorAxis::Y) = (height / 2) + GetVectorAxis(p, VectorAxis::Y);
    else GetVectorAxis(p, VectorAxis::Y) += height / 2;
    if (GetVectorAxis(p, VectorAxis::X) < 0) GetVectorAxis(p, VectorAxis::X) = (width / 2) + GetVectorAxis(p, VectorAxis::X);
    else GetVectorAxis(p, VectorAxis::X) += width / 2;
    GetVectorAxis(p, VectorAxis::Y) = height - GetVectorAxis(p, VectorAxis::Y);
}