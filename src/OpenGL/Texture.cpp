#include "Texture.hpp"
#include "OpenGL.hpp"

Texture::Texture(void) : unit(), texture(0) {}
Texture::Texture(const MathLib::Image& image, size_t unit) : unit(unit) {
    glGenTextures(1, &texture);
    Bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.GetWidth(), image.GetHeight(), 0, GL_ABGR_EXT, GL_UNSIGNED_BYTE, image.pixels.GetPointer());
    glGenerateMipmap(GL_TEXTURE_2D);
}
Texture::Texture(Texture&& other) : unit(other.unit), texture(other.texture) {
    other.texture = 0;
}
Texture::~Texture(void) {
    if (texture) glDeleteTextures(1, &texture);
}
Texture& Texture::operator=(Texture&& other) {
    if (texture) glDeleteTextures(1, &texture);
    unit = other.unit;
    texture = other.texture;
    other.texture = 0;
    return *this;
}
void Texture::Bind(void) const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, texture);
}