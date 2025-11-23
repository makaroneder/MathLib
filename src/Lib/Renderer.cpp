#include "Renderer.hpp"

namespace MathLib {
    Renderer::Renderer(size_t width, size_t height) : SaveableImage(width, height), position(CreateVector<num_t>(0, 0, 0)), pointMultiplier(100), fillGapsInFunctions(true), alphaPosition(0), image(nullptr) {}
    Renderer::~Renderer(void) {
        if (image) delete image;
    }
    void Renderer::DrawRectangle(ssize_t centerX, ssize_t centerY, size_t width, size_t height, uint32_t color) {
        const ssize_t leftX = centerX - width / 2;
        const ssize_t topY = centerY - height / 2;
        const ssize_t maxX = Min<ssize_t>(GetWidth() - leftX, width);
        const ssize_t maxY = Min<ssize_t>(GetHeight() - topY, height);
        const size_t minX = Max<ssize_t>(-topY, 0);
        const size_t minY = Max<ssize_t>(-leftX, 0);
        for (ssize_t y = minY; y < maxY; y++) {
            const ssize_t wy = topY + y;
            for (ssize_t x = minX; x < maxX; x++) {
                const ssize_t wx = leftX + x;
                AtUnsafe(wx, wy) = BlendColor(AtUnsafe(wx, wy), color, alphaPosition);
            }
        }
    }
    void Renderer::DrawImage(const Image& image, ssize_t centerX, ssize_t centerY, size_t width, size_t height, size_t startX, size_t startY) {
        const ssize_t leftX = centerX - width / 2;
        const ssize_t topY = centerY - height / 2;
        const ssize_t maxX = Min<ssize_t>(GetWidth() - leftX, width);
        const ssize_t maxY = Min<ssize_t>(GetHeight() - topY, height);
        const size_t minX = Max<ssize_t>(-topY, 0);
        const size_t minY = Max<ssize_t>(-leftX, 0);
        for (ssize_t y = minY; y < maxY; y++) {
            const ssize_t wy = topY + y;
            for (ssize_t x = minX; x < maxX; x++) {
                const ssize_t wx = leftX + x;
                AtUnsafe(wx, wy) = BlendColor(AtUnsafe(wx, wy), image.AtUnsafe(x + startX, y + startY), alphaPosition);
            }
        }
    }
    void Renderer::DrawImage(const Image& image, ssize_t centerX, ssize_t centerY) {
        DrawImage(image, centerX, centerY, image.GetWidth(), image.GetHeight(), 0, 0);
    }
    bool Renderer::DrawPartialImage(const Image& image, size_t startX, size_t endX, size_t startY, size_t endY, ssize_t centerX, ssize_t centerY) {
        if (endX < startX || endY < startY || endX >= image.GetWidth() || endY >= image.GetHeight()) return false;
        DrawImage(image, centerX, centerY, endX - startX, endY - startY, startX, startY);
        return true;
    }
    Event Renderer::WaitForEvent(void) {
        Event ret;
        while (ret.type == Event::Type::None) ret = GetEvent();
        return ret;
    }
    bool Renderer::Save(Writable& file) const {
        if (!image) return false;
        image->pixels = pixels;
        return image->Save(file);
    }
    bool Renderer::Load(Readable& file) {
        if (!image || !image->Load(file)) return false;
        pixels = image->pixels;
        return true;
    }
}