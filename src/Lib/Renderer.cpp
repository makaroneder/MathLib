#include "Renderer.hpp"

Renderer::Renderer(size_t w, size_t h) {
    width = w;
    height = h;
    pixels = Matrix<uint32_t>(width, height);
    position = CreateVector<num_t>(0, 0, 0);
    pointMultiplier = 100;
}
Renderer::~Renderer(void) {}
size_t Renderer::GetWidth(void) const {
    return width;
}
size_t Renderer::GetHeight(void) const {
    return height;
}
void Renderer::Fill(uint32_t color) {
    pixels.Fill(color);
}
bool Renderer::Update(void) {
    return UpdateInternal(pixels);
}
Event Renderer::WaitForEvent(void) {
    Event ret;
    while (ret.type == Event::Type::None) ret = GetEvent();
    return ret;
}
void Renderer::DrawAxis(uint32_t axisColor, uint32_t cellColor) {
    const num_t step = pointMultiplier / 100;
    const Matrix<num_t> start = (GetStart<num_t>() - position) * step;
    const Matrix<num_t> end = (GetEnd<num_t>() + position) * step;
    const num_t cellSize = 1;
    std::vector<num_t> setX = CreateRealNumberSet<num_t>();
    std::vector<num_t> setY = setX;
    for (num_t& i : setX) i += GetX(position);
    for (num_t& i : setY) i += GetY(position);
    for (num_t i = GetY(start); i <= GetY(end); i += cellSize) {
        DrawFunction<num_t>(GenerateFunction<num_t>([i](num_t) -> num_t { return i; }, setX), cellColor);
        DrawFunction<num_t>(GenerateFunction<num_t>([i](num_t) -> num_t { return i; }, setY, VectorAxis::Y, VectorAxis::X), cellColor);
    }
    DrawFunction<num_t>(GenerateFunction<num_t>([](num_t) -> num_t { return 0; }, setX), axisColor);
    DrawFunction<num_t>(GenerateFunction<num_t>([](num_t) -> num_t { return 0; }, setY, VectorAxis::Y, VectorAxis::X), axisColor);
}