#include "Aseprite.hpp"
#include "../../Color.hpp"
#include "../../Memory.hpp"
#include "AsepriteHeader.hpp"
#include "AsepriteFrameHeader.hpp"
#include "AsepriteChunkHeader.hpp"
#include "AsepriteImageCelChunk.hpp"

namespace MathLib {
    Aseprite::Aseprite(size_t width, size_t height) : SaveableImage(width, height) {
        EmptyBenchmark
    }
    bool Aseprite::Save(Writeable& file) const {
        // TODO:
        (void)file;
        StartBenchmark
        ReturnFromBenchmark(false);
    }
    bool Aseprite::Load(Readable& file) {
        StartBenchmark
        AsepriteHeader header;
        if (!file.Read(header) || !header.IsValid() || header.bpp != 32 || header.frames != 1) ReturnFromBenchmark(false);
        AsepriteFrameHeader frame;
        if (!file.Read(frame) || !frame.IsValid()) ReturnFromBenchmark(false);
        const uint32_t chunks = frame.chunks16 == UINT16_MAX && frame.chunks32 ? frame.chunks32 : frame.chunks16;
        for (size_t i = 0; i < chunks; i++) {
            uint32_t size;
            if (!file.Read(size) || size < sizeof(AsepriteChunkHeader)) ReturnFromBenchmark(false);
            uint8_t buff[size];
            MemoryCopy(&size, buff, sizeof(uint32_t));
            if (!file.ReadBuffer(buff + sizeof(uint32_t), size - sizeof(uint32_t))) ReturnFromBenchmark(false);
            const AsepriteChunkHeader* chunk = (const AsepriteChunkHeader*)buff;
            switch (chunk->type) {
                case AsepriteChunkHeader::Type::Cel: {
                    const AsepriteCelChunk* cel = (const AsepriteCelChunk*)chunk;
                    if (cel->type == AsepriteCelChunk::Type::Raw || cel->type == AsepriteCelChunk::Type::CompressedImage) {
                        const AsepriteImageCelChunk* img = (const AsepriteImageCelChunk*)cel;
                        pixels = Matrix<uint32_t>(img->width, img->height);
                        for (size_t y = 0; y < img->height; y++)
                            for (size_t x = 0; x < img->width; x++)
                                pixels.At(x, y) = img->pixels[y * img->width + x];
                    }
                }
                default: break;
            }
        }
        ReturnFromBenchmark(true);
    }
}