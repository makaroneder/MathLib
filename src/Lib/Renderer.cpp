#include "Renderer.hpp"

Renderer::Renderer(void) {
    position = CreateVector<num_t>(0, 0, 0);
    pointMultiplier = 100;
}
Renderer::~Renderer(void) {}
Event Renderer::WaitForEvent(void) {
    Event ret;
    while (ret.type == Event::Type::None) ret = GetEvent();
    return ret;
}
bool Renderer::Fill(uint32_t color) {
    num_t w = GetWidth() / 2;
    num_t h = GetHeight() / 2;
    for (num_t x = 0; x < w; x++)
        for (num_t y = 0; y < h; y++)
            if (!SetPixel(CreateVector<num_t>(x, y, 0), color)) return false;
    return true;
}
bool Renderer::DrawAxis(uint32_t axisColor, uint32_t cellColor) {
    const num_t step = pointMultiplier / 100;
    const Matrix<num_t> start = (GetStart<num_t>() - position) * step;
    const Matrix<num_t> end = (GetEnd<num_t>() + position) * step;
    const num_t cellSize = 1;
    std::vector<num_t> setX = CreateRealNumberSet<num_t>();
    std::vector<num_t> setY = setX;
    for (num_t& i : setX) i += GetX(position);
    for (num_t& i : setY) i += GetY(position);
    for (num_t i = GetY(start); i <= GetY(end); i += cellSize) {
        if (!DrawFunction<num_t>(GenerateFunction<num_t>(ConstFunction(num_t, i), setX, {}), cellColor)) return false;
        if (!DrawFunction<num_t>(GenerateFunction<num_t>(ConstFunction(num_t, i), setY, {}, VectorAxis::Y, VectorAxis::X), cellColor)) return false;
    }
    return (
        DrawFunction<num_t>(GenerateFunction<num_t>([](num_t) { return 0; }, setX, {}), axisColor) &&
        DrawFunction<num_t>(GenerateFunction<num_t>([](num_t) { return 0; }, setY, {}, VectorAxis::Y, VectorAxis::X), axisColor)
    );
}