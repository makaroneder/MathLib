#include "Test.hpp"

Test::Test(size_t w, size_t h) {
    width = w;
    height = h;
    failed = 0;
    values = {};
}
bool Test::Update(void) {
    return true;
}
void Test::Quit(void) { }
size_t Test::GetWidth(void) const {
    return width;
}
size_t Test::GetHeight(void) const {
    return height;
}
bool Test::SetPixel(Vector2<num_t> p, uint32_t color) {
    (void)color;
    if (std::find(values.begin(), values.end(), p) == values.end()) failed++;
    return true;
}
bool Test::DrawLine(Vector2<num_t> p1, Vector2<num_t> p2, uint32_t color) {
    (void)p1;
    return SetPixel(p2, color);
}