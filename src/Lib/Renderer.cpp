#include "Renderer.hpp"

Renderer::Renderer(size_t w, size_t h) : position(CreateVector<num_t>(0, 0, 0)), pointMultiplier(100), width(w), height(h), pixels(Matrix<uint32_t>(w, h)) { }
Renderer::~Renderer(void) {}
size_t Renderer::GetWidth(void) const {
    return width;
}
size_t Renderer::GetHeight(void) const {
    return height;
}
Matrix<uint32_t> Renderer::GetPixels(void) const {
    return pixels;
}
void Renderer::Fill(uint32_t color) {
    pixels.Fill(color);
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
bool Renderer::Save(FILE* file) const {
    fprintf(file, "P6\n%lu %lu 255\n", width, height);
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            const Color c = pixels.At(x, y);
            if (fwrite(&c.rgba.r, sizeof(uint8_t), 1, file) != 1) return false;
            if (fwrite(&c.rgba.g, sizeof(uint8_t), 1, file) != 1) return false;
            if (fwrite(&c.rgba.b, sizeof(uint8_t), 1, file) != 1) return false;
        }
    }
    return true;
}
bool Renderer::Load(FILE* file) {
    fscanf(file, "P6\n%lu %lu 255\n", &width, &height);
    pixels = Matrix<uint32_t>(width, height);
    position = CreateVector<num_t>(0, 0, 0);
    pointMultiplier = 100;
    fseek(file, 9 + std::to_string(width).size() + std::to_string(height).size(), SEEK_SET);
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            Color c;
            if (fread(&c.rgba.r, sizeof(uint8_t), 1, file) != 1) return false;
            if (fread(&c.rgba.g, sizeof(uint8_t), 1, file) != 1) return false;
            if (fread(&c.rgba.b, sizeof(uint8_t), 1, file) != 1) return false;
            pixels.At(x, y) = c.hex;
        }
    }
    return true;
}