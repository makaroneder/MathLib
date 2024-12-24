#include "P6.hpp"
#include "../../Color.hpp"

namespace MathLib {
    P6::P6(size_t width, size_t height) : SaveableImage(width, height) {
        EmptyBenchmark
    }
    bool P6::Save(Writeable& file) const {
        StartBenchmark
        if (!file.Puts("P6\n") || !file.Puts(ToString(pixels.GetWidth())) || !file.Puts(" ") || !file.Puts(ToString(pixels.GetHeight())) || !file.Puts(" 255\n")) ReturnFromBenchmark(false);
        for (size_t y = 0; y < pixels.GetHeight(); y++) {
            for (size_t x = 0; x < pixels.GetWidth(); x++) {
                const Color pixel = pixels.At(x, y);
                const uint8_t tmp[] = {
                    pixel.rgba.r,
                    pixel.rgba.g,
                    pixel.rgba.b,
                };
                if (!file.WriteBuffer(tmp, SizeOfArray(tmp) * sizeof(uint8_t))) ReturnFromBenchmark(false);
            }
        }
        ReturnFromBenchmark(true);
    }
    bool P6::Load(Readable& file) {
        StartBenchmark
        char tmp[5] = { '\0', };
        if (!file.ReadBuffer(tmp, 3 * sizeof(char)) || String(tmp) != "P6\n") ReturnFromBenchmark(false);
        const size_t width = StringToNumber(file.ReadUntil(' '));
        const Expected<char> tmp2 = file.Getc();
        if (!tmp2.HasValue() || tmp2.Get() != ' ') ReturnFromBenchmark(false);
        const size_t height = StringToNumber(file.ReadUntil(' '));
        if (!file.ReadBuffer(tmp, 4 * sizeof(char)) || String(tmp) != " 255") ReturnFromBenchmark(false);
        pixels = Matrix<uint32_t>(width, height);
        for (size_t y = 0; y < height; y++) {
            for (size_t x = 0; x < width; x++) {
                uint8_t tmp[3] = { 0, 0, 0, };
                if (!file.ReadBuffer(tmp, SizeOfArray(tmp) * sizeof(uint8_t))) ReturnFromBenchmark(false);
                pixels.At(x, y) = Color(tmp[0], tmp[1], tmp[2], UINT8_MAX).hex;
            }
        }
        ReturnFromBenchmark(true);
    }
}