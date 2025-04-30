#ifndef MathLib_Image_PNG_IHDRChunk_H
#define MathLib_Image_PNG_IHDRChunk_H
#include "PNGChunk.hpp"

namespace MathLib {
    struct IHDRChunk : PNGChunk {
        static constexpr const char* expectedType = "IHDR";
        enum class ColorType : uint8_t {
            Grayscale = 0,
            RGB = 2,
            Indexed,
            GrayscaleAlpha,
            RGBA = 6,
        };
        enum class CompressionMethod : uint8_t {
            Deflate = 0,
        };
        enum class FilterMethod : uint8_t {
            None = 0,
        };
        enum class InterlaceMethod : uint8_t {
            None = 0,
            Adam7,
        };
        uint32_t width;
        uint32_t height;
        uint8_t bitDepth;
        ColorType colorType;
        CompressionMethod compressionMethod;
        FilterMethod filterMethod;
        InterlaceMethod interlaceMethod;

        IHDRChunk(uint32_t width, uint32_t height, uint8_t bitDepth, ColorType colorType, CompressionMethod compressionMethod, FilterMethod filterMethod, InterlaceMethod interlaceMethod);
    } __attribute__((packed));
}

#endif