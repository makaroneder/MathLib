#include "Renderer.hpp"

Renderer::Renderer(void) {
    position = CreateVector3<num_t>(0, 0, 0);
    pointMultiplier = 100;
}
Renderer::~Renderer(void) {}
bool Renderer::DrawAxis(uint32_t color) {
    // TODO: Calculate the render starting and ending point
    const Vector3<num_t> start = GetStart<num_t>() * 10;
    const Vector3<num_t> end = GetEnd<num_t>() * 10;
    const num_t step = pointMultiplier / 100;
    const num_t cellSize = 1;
    bool ret = true;
    for (num_t y = (GetVectorAxis(start, VectorAxis::Y) - GetVectorAxis(position, VectorAxis::Y)) * step; y <= (GetVectorAxis(end, VectorAxis::Y) + GetVectorAxis(position, VectorAxis::Y)) * step && ret; y += cellSize)
        for (num_t x = (GetVectorAxis(start, VectorAxis::X) - GetVectorAxis(position, VectorAxis::X)) * step; x <= (GetVectorAxis(end, VectorAxis::X) + GetVectorAxis(position, VectorAxis::X)) * step && ret; x += cellSize)
            ret = DrawLine(Line<num_t>(CreateVector3<num_t>(x, y, 0) - position, CreateVector3<num_t>(x + cellSize, y, 0) - position), FloatsEqual<num_t>(y, 0, 1 / pointMultiplier) ? color : 0x808080ff) && DrawLine(Line<num_t>(CreateVector3<num_t>(x, y, 0) - position, CreateVector3<num_t>(x, y + cellSize, 0) - position), FloatsEqual<num_t>(x, 0, 1 / pointMultiplier) ? color : 0x808080ff);
    return ret;
}