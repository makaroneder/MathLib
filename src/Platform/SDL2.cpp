#include "SDL2.hpp"
#include <Color.hpp>
#include <iostream>

SDL2Renderer::SDL2Renderer(const char* title, size_t w, size_t h) : Renderer() {
    width = w;
    height = h;
    if (SDL_Init(SDL_INIT_VIDEO) != 0) throw std::runtime_error("Failed to initialize renderer");
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
    if (window == nullptr) throw std::runtime_error("Failed to initialize window");
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == nullptr) throw std::runtime_error("Failed to initialize renderer");
}
SDL2Renderer::~SDL2Renderer(void) {
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
bool SDL2Renderer::SetPixel(Vector3<num_t> p, uint32_t color) {
    if (!SetColor(color)) return false;
    ToInternalPoint(p);
    return SDL_RenderDrawPointF(renderer, GetVectorAxis(p, VectorAxis::X), GetVectorAxis(p, VectorAxis::Y)) == 0;
}
bool SDL2Renderer::DrawLine(Line<num_t> line, uint32_t color) {
    if (!SetColor(color)) return false;
    ToInternalPoint(line.start);
    ToInternalPoint(line.end);
    return SDL_RenderDrawLineF(renderer, GetVectorAxis(line.start, VectorAxis::X), GetVectorAxis(line.start, VectorAxis::Y), GetVectorAxis(line.end, VectorAxis::X), GetVectorAxis(line.end, VectorAxis::Y)) == 0;
}
bool SDL2Renderer::Update(void) {
    SDL_RenderPresent(renderer);
    return SetColor(0) && SDL_RenderClear(renderer) == 0;
}
SDL_Event SDL2Renderer::GetEvent(void) {
    SDL_Event event;
    if (SDL_PollEvent(&event) != 0) return event;
    else {
        event.type = SDL_FIRSTEVENT;
        return event;
    }
}
bool SDL2Renderer::SetColor(uint32_t c) {
    Color color = Color(c);
    return SDL_SetRenderDrawColor(renderer, color.rgba.r, color.rgba.g, color.rgba.b, color.rgba.a) == 0;
}
void SDL2Renderer::ToInternalPoint(Vector3<num_t>& p) {
    Vector2<num_t> tmp = ConvertVector3ToVector2<num_t>(p);
    p = CreateVector3<num_t>(GetVectorAxis(tmp, VectorAxis::X) * pointMultiplier, GetVectorAxis(tmp, VectorAxis::Y) * pointMultiplier, 0);
    if (GetVectorAxis(p, VectorAxis::Y) < 0) GetVectorAxis(p, VectorAxis::Y) = (height / 2) + GetVectorAxis(p, VectorAxis::Y);
    else GetVectorAxis(p, VectorAxis::Y) += height / 2;
    if (GetVectorAxis(p, VectorAxis::X) < 0) GetVectorAxis(p, VectorAxis::X) = (width / 2) + GetVectorAxis(p, VectorAxis::X);
    else GetVectorAxis(p, VectorAxis::X) += width / 2;
    GetVectorAxis(p, VectorAxis::Y) = height - GetVectorAxis(p, VectorAxis::Y);
}