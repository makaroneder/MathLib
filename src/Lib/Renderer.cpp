#include "Renderer.hpp"

namespace MathLib {
    Renderer::Renderer(size_t width, size_t height) : SaveableImage(width, height), position(CreateVector<num_t>(0, 0, 0)), pointMultiplier(100), fillGapsInFunctions(true), alphaPosition(0), image(nullptr) {}
    Renderer::~Renderer(void) {
        if (image) delete image;
    }
    void Renderer::FillRectangle(ssize_t centerX, ssize_t centerY, size_t width, size_t height, uint32_t color) {
        const ssize_t leftX = centerX - width / 2;
        const ssize_t topY = centerY - height / 2;
        const ssize_t maxX = Min<ssize_t>(GetWidth() - leftX, width);
        const ssize_t maxY = Min<ssize_t>(GetHeight() - topY, height);
        const size_t minX = Max<ssize_t>(-leftX, 0);
        const size_t minY = Max<ssize_t>(-topY, 0);
        for (ssize_t y = minY; y < maxY; y++) {
            const ssize_t wy = topY + y;
            for (ssize_t x = minX; x < maxX; x++) {
                const ssize_t wx = leftX + x;
                AtUnsafe(wx, wy) = BlendColor(AtUnsafe(wx, wy), color, alphaPosition);
            }
        }
    }
    void Renderer::DrawRectangle(ssize_t centerX, ssize_t centerY, size_t width, size_t height, uint32_t color) {
        const ssize_t leftX = centerX - width / 2;
        const ssize_t rightX = centerX + width / 2;
        const ssize_t topY = centerY - height / 2;
        const ssize_t bottomY = centerY + height / 2;
        DrawLinePararellToOX(leftX, rightX, topY, color);
        DrawLinePararellToOX(leftX, rightX, bottomY, color);
        DrawLinePararellToOY(topY, bottomY, leftX, color);
        DrawLinePararellToOY(topY, bottomY, rightX, color);
    }
    void Renderer::DrawLinePararellToOX(ssize_t startX, ssize_t endX, ssize_t y, uint32_t color) {
        if (y < 0 || (size_t)y >= GetHeight()) return;
        if (endX < startX) Swap<ssize_t>(startX, endX);
        const size_t min = Max<ssize_t>(startX, 0);
        const ssize_t max = Min<ssize_t>(endX, GetWidth());
        for (ssize_t x = min; x < max; x++) AtUnsafe(x, y) = BlendColor(AtUnsafe(x, y), color, alphaPosition);
    }
    void Renderer::DrawLinePararellToOY(ssize_t startY, ssize_t endY, ssize_t x, uint32_t color) {
        if (x < 0 || (size_t)x >= GetWidth()) return;
        if (endY < startY) Swap<ssize_t>(startY, endY);
        const size_t min = Max<ssize_t>(startY, 0);
        const ssize_t max = Min<ssize_t>(endY, GetHeight());
        for (ssize_t y = min; y < max; y++) AtUnsafe(x, y) = BlendColor(AtUnsafe(x, y), color, alphaPosition);
    }
    void Renderer::DrawGrid(ssize_t centerX, ssize_t centerY, size_t width, size_t height, size_t cellsX, size_t cellsY, uint32_t color) {
        const ssize_t leftX = centerX - width * cellsX / 2;
        const ssize_t topY = centerY - height * cellsY / 2;
        for (size_t y = 0; y < cellsY; y++) {
            const ssize_t startY = topY + y * height;
            const ssize_t endY = startY + height;
            for (size_t x = 0; x < cellsX; x++) {
                const ssize_t startX = leftX + x * width;
                const ssize_t endX = startX + width;
                DrawLinePararellToOX(startX, endX, startY, color);
                DrawLinePararellToOX(startX, endX, endY, color);
                DrawLinePararellToOY(startY, endY, startX, color);
                DrawLinePararellToOY(startY, endY, endX, color);
            }
        }
    }
    void Renderer::DrawImage(const Image& image, ssize_t centerX, ssize_t centerY, size_t width, size_t height, size_t startX, size_t startY) {
        const ssize_t leftX = centerX - width / 2;
        const ssize_t topY = centerY - height / 2;
        const ssize_t maxX = Min<ssize_t>(GetWidth() - leftX, width);
        const ssize_t maxY = Min<ssize_t>(GetHeight() - topY, height);
        const size_t minX = Max<ssize_t>(-leftX, 0);
        const size_t minY = Max<ssize_t>(-topY, 0);
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
    void Renderer::DrawStackedImage(const Video& images, ssize_t centerX, ssize_t centerY, ssize_t diff) {
        const size_t size = images.GetSize();
        centerY -= size * diff;
        for (size_t i = size; i; i--) DrawImage(images.AtUnsafe(i - 1), centerX, centerY + i * diff);
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