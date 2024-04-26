#include "PPMRenderer.hpp"

PPMRenderer::PPMRenderer(const char* outputFile, size_t w, size_t h) : Renderer(w, h) {
    path = outputFile;
}
bool PPMRenderer::UpdateInternal(Matrix<uint32_t> pixels) {
    FILE* file = fopen(path, "w");
    if (file == nullptr) return false;
    const size_t width = GetWidth();
    const size_t height = GetHeight();
    fprintf(file, "P6\n%lu %lu 255\n", width, height);
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            const Color c = pixels.At(x, y);
            if (fwrite(&c.rgba.r, sizeof(uint8_t), 1, file) != 1) return false;
            if (fwrite(&c.rgba.g, sizeof(uint8_t), 1, file) != 1) return false;
            if (fwrite(&c.rgba.b, sizeof(uint8_t), 1, file) != 1) return false;
        }
    }
    return fclose(file) == 0;
}
Event PPMRenderer::GetEvent(void) {
    return Event();
}