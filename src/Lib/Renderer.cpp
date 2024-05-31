#include "Renderer.hpp"

Renderer::Renderer(size_t w, size_t h) : position(CreateVector<num_t>(0, 0, 0)), pointMultiplier(100), pixels(Matrix<uint32_t>(w, h)) { }
Renderer::~Renderer(void) {}
size_t Renderer::GetWidth(void) const {
    return pixels.GetWidth();
}
size_t Renderer::GetHeight(void) const {
    return pixels.GetHeight();
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
    const Matrix<num_t> start = (GetStart<num_t>()) * step;
    const Matrix<num_t> end = (GetEnd<num_t>()) * step;
    const num_t cellSize = 1;
    for (num_t i = GetY(start); i <= GetY(end); i += cellSize) {
        DrawLine<num_t>(Line<num_t>(CreateVector<num_t>(GetX(start), i, 0) + position, CreateVector<num_t>(GetX(end), i, 0) + position), cellColor);
        DrawLine<num_t>(Line<num_t>(CreateVector<num_t>(i, GetY(start), 0) + position, CreateVector<num_t>(i, GetY(end), 0) + position), cellColor);
    }
    DrawLine<num_t>(Line<num_t>(CreateVector<num_t>(GetX(start) + GetX(position), 0, 0), CreateVector<num_t>(GetX(end) + GetX(position), 0, 0)), axisColor);
    DrawLine<num_t>(Line<num_t>(CreateVector<num_t>(0, GetY(start) + GetY(position), 0), CreateVector<num_t>(0, GetY(end) + GetY(position), 0)), axisColor);
}
bool Renderer::Save(FileSystem& fileSystem, size_t file) const {
    if (!fileSystem.Puts(file, "P6\n") || !fileSystem.Puts(file, ToString(pixels.GetWidth())) || !fileSystem.Puts(file, " ") || !fileSystem.Puts(file, ToString(pixels.GetHeight())) || !fileSystem.Puts(file, " 255\n")) return false;
    for (size_t y = 0; y < pixels.GetHeight(); y++) {
        for (size_t x = 0; x < pixels.GetWidth(); x++) {
            const Color c = pixels.At(x, y);
            if (!fileSystem.Write(file, &c.rgba.r, sizeof(uint8_t)) || !fileSystem.Write(file, &c.rgba.g, sizeof(uint8_t)) || !fileSystem.Write(file, &c.rgba.b, sizeof(uint8_t))) return false;
        }
    }
    return true;
}
bool Renderer::Load(FileSystem& fileSystem, size_t file) {
    char tmp[5] = { '\0', };
    if (!fileSystem.Read(file, tmp, 3 * sizeof(char)) || String(tmp) != "P6\n") return false;
    const size_t width = StringToNumber(fileSystem.ReadUntil(file, ' '));
    if (fileSystem.Getc(file) != ' ') return false;
    const size_t height = StringToNumber(fileSystem.ReadUntil(file, ' '));
    if (!fileSystem.Read(file, tmp, 4 * sizeof(char)) || String(tmp) != " 255") return false;
    pixels = Matrix<uint32_t>(width, height);
    position = CreateVector<num_t>(0, 0, 0);
    pointMultiplier = 100;
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            Color c;
            if (!fileSystem.Read(file, &c.rgba.r, sizeof(uint8_t)) || !fileSystem.Read(file, &c.rgba.g, sizeof(uint8_t)) || !fileSystem.Read(file, &c.rgba.b, sizeof(uint8_t))) return false;
            pixels.At(x, y) = c.hex;
        }
    }
    return true;
}