#ifndef Texture_H
#define Texture_H
#include <Image/Image.hpp>

struct Texture {
    Texture(void);
    Texture(const MathLib::Image& image, size_t unit);
    Texture(const Texture& other) = delete;
    Texture(Texture&& other);
    ~Texture(void);
    Texture& operator=(const Texture& other) = delete;
    Texture& operator=(Texture&& other);
    void Bind(void) const;

    private:
    size_t unit;
    uint32_t texture;
};

#endif