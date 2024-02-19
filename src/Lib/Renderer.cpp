#include "Renderer.hpp"

Renderer::Renderer(void) {
    position = CreateVector3<num_t>(0, 0, 0);
    pointMultiplier = 100;
}
Renderer::~Renderer(void) {}
bool Renderer::DrawAxis(uint32_t color) {
    const Vector2<num_t> start = GetStart<num_t>();
    const Vector2<num_t> end = GetEnd<num_t>();
    // TODO: Resize lines to fit the screen with modified position
    return (
        DrawLine(Line<num_t>(CreateVector3<num_t>(GetVectorAxis(start, VectorAxis::X), 0, 0) - position, CreateVector3<num_t>(GetVectorAxis(end, VectorAxis::X), 0, 0) - position), color) &&
        DrawLine(Line<num_t>(CreateVector3<num_t>(0, GetVectorAxis(start, VectorAxis::Y), 0) - position, CreateVector3<num_t>(0, GetVectorAxis(end, VectorAxis::Y), 0) - position), color)
    );
}