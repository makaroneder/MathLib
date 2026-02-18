#include "IHDRChunk.hpp"
#include "../../Utils.hpp"

namespace MathLib {
    IHDRChunk::IHDRChunk(uint32_t width, uint32_t height, uint8_t bitDepth, ColorType colorType, CompressionMethod compressionMethod, FilterMethod filterMethod, InterlaceMethod interlaceMethod) : PNGChunk(sizeof(IHDRChunk) - sizeof(PNGChunk), expectedType), width(SwapEndian32(width)), height(SwapEndian32(height)), bitDepth(bitDepth), colorType(colorType), compressionMethod(compressionMethod), filterMethod(filterMethod), interlaceMethod(interlaceMethod) {}
}