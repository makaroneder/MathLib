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
bool Test::Update(void) {
    return true;
}
bool Test::SetPixel(Matrix<num_t> pixel, uint32_t color) {
    (void)color;
    if (std::find(values.begin(), values.end(), pixel + position) == values.end()) failed++;
    return true;
}
Event Test::GetEvent(void) {
    return Event();
}