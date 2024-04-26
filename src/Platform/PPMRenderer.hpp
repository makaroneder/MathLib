#ifndef PPMRenderer_H
#define PPMRenderer_H
#include <Renderer.hpp>

struct PPMRenderer : Renderer {
    PPMRenderer(const char* outputFile, size_t w, size_t h);
    virtual bool UpdateInternal(Matrix<uint32_t> pixels) override;
    virtual Event GetEvent(void) override;

    private:
    const char* path;
};

#endif