#include "Test.hpp"
// TODO: Check if function was called expected number of times

Test::Test(size_t w, size_t h) : Renderer() {
    width = w;
    height = h;
    failed = 0;
    values = {};
}
size_t Test::GetWidth(void) const {
    return width;
}
size_t Test::GetHeight(void) const {
    return height;
}
bool Test::SetPixel(Vector3<num_t> p, uint32_t color) {
    (void)color;
    if (std::find(values.begin(), values.end(), p) == values.end()) failed++;
    return true;
}
bool Test::DrawLine(Line<num_t> line, uint32_t color) {
    return SetPixel(line.end, color);
}