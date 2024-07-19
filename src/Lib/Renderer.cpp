#include "Renderer.hpp"

Renderer::Renderer(const size_t& w, const size_t& h) : Image(w, h), position(CreateVector<num_t>(0, 0, 0)), pointMultiplier(100), image(nullptr) {}
Renderer::~Renderer(void) {
    if (image) delete image;
}
Event Renderer::WaitForEvent(void) {
    Event ret;
    while (ret.type == Event::Type::None) ret = GetEvent();
    return ret;
}
void Renderer::DrawAxis(const uint32_t& axisColor, const uint32_t& cellColor) {
    const num_t cellSize = 1;
    const Matrix<num_t> start = GetStart<num_t>();
    const Matrix<num_t> end = GetEnd<num_t>();
    Matrix<num_t> newStart = CreateVector<num_t>(0, 0, 0);
    Matrix<num_t> newEnd = CreateVector<num_t>(0, 0, 0);
    while (GetX(newStart) > GetX(start)) GetX(newStart) -= cellSize;
    while (GetY(newStart) > GetY(start)) GetY(newStart) -= cellSize;
    while (GetX(newEnd) < GetX(end)) GetX(newEnd) += cellSize;
    while (GetY(newEnd) < GetY(end)) GetY(newEnd) += cellSize;
    for (num_t i = GetY(newStart); i <= GetY(newEnd); i += cellSize) {
        DrawLine<num_t>(Line<num_t>(CreateVector<num_t>(GetX(newStart), i, 0) + position, CreateVector<num_t>(GetX(newEnd), i, 0) + position), cellColor);
        DrawLine<num_t>(Line<num_t>(CreateVector<num_t>(i, GetY(newStart), 0) + position, CreateVector<num_t>(i, GetY(newEnd), 0) + position), cellColor);
    }
    DrawLine<num_t>(Line<num_t>(CreateVector<num_t>(GetX(start) + GetX(position), 0, 0), CreateVector<num_t>(GetX(end) + GetX(position), 0, 0)), axisColor);
    DrawLine<num_t>(Line<num_t>(CreateVector<num_t>(0, GetY(start) + GetY(position), 0), CreateVector<num_t>(0, GetY(end) + GetY(position), 0)), axisColor);
}
bool Renderer::Save(ByteDevice& file) const {
    if (!image) return false;
    image->pixels = pixels;
    return image->Save(file);
}
bool Renderer::Load(ByteDevice& file) {
    if (!image || !image->Load(file)) return false;
    pixels = image->pixels;
    return true;
}