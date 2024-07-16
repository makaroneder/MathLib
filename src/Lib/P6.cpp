#include "P6.hpp"
#include "Color.hpp"

P6::P6(const size_t& w, const size_t& h) : Image(w, h) {}
bool P6::Save(ByteDevice& file) const {
    if (!file.Puts("P6\n") || !file.Puts(ToString(pixels.GetWidth())) || !file.Puts(" ") || !file.Puts(ToString(pixels.GetHeight())) || !file.Puts(" 255\n")) return false;
    for (size_t y = 0; y < pixels.GetHeight(); y++) {
        for (size_t x = 0; x < pixels.GetWidth(); x++) {
            const Color c = pixels.At(x, y);
            const uint8_t tmp[] = {
                c.rgba.r,
                c.rgba.g,
                c.rgba.b,
            };
            if (!file.WriteBuffer(tmp, 3 * sizeof(uint8_t))) return false;
        }
    }
    return true;

}
bool P6::Load(ByteDevice& file) {
    char tmp[5] = { '\0', };
    if (!file.ReadBuffer(tmp, 3 * sizeof(char)) || String(tmp) != "P6\n") return false;
    const size_t width = StringToNumber(file.ReadUntil(' '));
    if (file.Getc() != ' ') return false;
    const size_t height = StringToNumber(file.ReadUntil(' '));
    if (!file.ReadBuffer(tmp, 4 * sizeof(char)) || String(tmp) != " 255") return false;
    pixels = Matrix<uint32_t>(width, height);
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            uint8_t tmp[3] = { 0, 0, 0, };
            if (!file.ReadBuffer(tmp, 3 * sizeof(uint8_t))) return false;
            pixels.At(x, y) = Color(tmp[0], tmp[1], tmp[2], UINT8_MAX).hex;
        }
    }
    return true;
}