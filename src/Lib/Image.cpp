#include "Image.hpp"

namespace MathLib {
    Image::Image(size_t width, size_t height) : pixels(Matrix<uint32_t>(width, height)) { }
    SaveableImage::SaveableImage(size_t width, size_t height) : Image(width, height) {}
    size_t Image::GetWidth(void) const {
        return pixels.GetWidth();
    }
    size_t Image::GetHeight(void) const {
        return pixels.GetHeight();
    }
    void Image::Fill(uint32_t color) {
        pixels.Fill(color);
    }
    uint32_t& Image::At(size_t x, size_t y) {
        return pixels.At(x, y);
    }
    uint32_t Image::At(size_t x, size_t y) const {
        return pixels.At(x, y);
    }
    Image Image::Resize(size_t xMultiplier, size_t yMultiplier) const {
        const size_t width = GetWidth();
        const size_t heigth = GetHeight();
        Image ret = Image(width * xMultiplier, heigth * yMultiplier);
        for (size_t y1 = 0; y1 < heigth; y1++)
            for (size_t y2 = 0; y2 < yMultiplier; y2++)
                for (size_t x1 = 0; x1 < width; x1++)
                    for (size_t x2 = 0; x2 < xMultiplier; x2++)
                        ret.At(x1 * xMultiplier + x2, y1 * yMultiplier + y2) = At(x1, y1);
        return ret;
    }
}