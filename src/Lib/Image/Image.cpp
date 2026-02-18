#include "Image.hpp"

namespace MathLib {
    Image::Image(void) : pixels() {}
    Image::Image(size_t width, size_t height) : pixels(Matrix<uint32_t>(width, height)) {}
    size_t Image::GetWidth(void) const {
        return pixels.GetWidth();
    }
    size_t Image::GetHeight(void) const {
        return pixels.GetHeight();
    }
    size_t Image::GetSize(void) const {
        return pixels.GetWidth() * pixels.GetHeight();
    }
    template <> void Image::FillBytes(uint8_t value) {
        pixels.FillBytes8(value);
    }
    template <> void Image::FillBytes(uint16_t value) {
        pixels.FillBytes16(value);
    }
    template <> void Image::FillBytes(uint32_t value) {
        pixels.FillBytes32(value);
    }
    template <> void Image::FillBytes(uint64_t value) {
        pixels.FillBytes64(value);
    }
    void Image::Fill(uint32_t color) {
        pixels.FillBytes32(color);
    }
    uint32_t& Image::AtUnsafe(size_t x, size_t y) {
        return pixels.AtUnsafe(x, y);
    }
    uint32_t Image::AtUnsafe(size_t x, size_t y) const {
        return pixels.AtUnsafe(x, y);
    }
    uint32_t& Image::At(size_t x, size_t y) {
        return pixels.At(x, y);
    }
    uint32_t Image::At(size_t x, size_t y) const {
        return pixels.At(x, y);
    }
    void Image::CopyFromBuffer(const uint32_t* buffer) {
        pixels.CopyFromBuffer(buffer);
    }
    bool Image::CopyFromBuffer(const Collection<uint32_t>& buffer) {
        return pixels.CopyFromBuffer(buffer);
    }
    bool Image::CopyFromBuffer(const Matrix<uint32_t>& buffer) {
        return pixels.CopyFromBuffer(buffer);
    }
    bool Image::CopyFromBuffer(const Image& buffer) {
        return pixels.CopyFromBuffer(buffer.pixels);
    }
    Image Image::Resize(size_t xMultiplier, size_t yMultiplier) const {
        const size_t width = GetWidth();
        const size_t height = GetHeight();
        Image ret = Image(width * xMultiplier, height * yMultiplier);
        for (size_t y1 = 0; y1 < height; y1++)
            for (size_t y2 = 0; y2 < yMultiplier; y2++)
                for (size_t x1 = 0; x1 < width; x1++)
                    for (size_t x2 = 0; x2 < xMultiplier; x2++)
                        ret.AtUnsafe(x1 * xMultiplier + x2, y1 * yMultiplier + y2) = AtUnsafe(x1, y1);
        return ret;
    }
    Image Image::RotateUpsideDown(void) const {
        const size_t width = GetWidth();
        const size_t height = GetHeight();
        Image ret = Image(width, height);
        for (size_t y = 0; y < height; y++)
            if (!ret.pixels.CopyRow(pixels, y, height - 1 - y)) return Image();
        return ret;
    }
    Image Image::SwapXY(void) const {
        const size_t width = GetWidth();
        const size_t height = GetHeight();
        Image ret = Image(height, width);
        for (size_t y = 0; y < width; y++)
            for (size_t x = 0; x < height; x++) ret.AtUnsafe(x, y) = AtUnsafe(y, x);
        return ret;
    }
    Image Image::SwapXYAndRotateUpsideDown(void) const {
        const size_t width = GetWidth();
        const size_t height = GetHeight();
        Image ret = Image(height, width);
        for (size_t y = 0; y < width; y++)
            for (size_t x = 0; x < height; x++) ret.AtUnsafe(x, y) = AtUnsafe(y, height - 1 - x);
        return ret;
    }
    bool Image::Equals(const Image& other) const {
        return pixels == other.pixels;
    }
}