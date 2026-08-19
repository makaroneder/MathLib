#ifndef Texture_H
#define Texture_H
#include <Image/Image.hpp>

struct Texture {
    Texture(const MathLib::Image& image, size_t unit);
    ~Texture(void);
    void Bind(void) const;

    private:
    size_t unit;
    uint32_t texture;
};

#endif