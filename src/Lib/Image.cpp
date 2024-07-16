#include "Image.hpp"

Image::Image(const size_t& w, const size_t& h) : pixels(Matrix<uint32_t>(w, h)) { }
size_t Image::GetWidth(void) const {
    return pixels.GetWidth();
}
size_t Image::GetHeight(void) const {
    return pixels.GetHeight();
}
void Image::Fill(const uint32_t& color) {
    pixels.Fill(color);
}