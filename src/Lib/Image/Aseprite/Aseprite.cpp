#include "Aseprite.hpp"
#include "../../Color.hpp"
#include "../../Memory.hpp"
#include "../../Bitmap.hpp"
#include "AsepriteHeader.hpp"
#include "../../ExternArray.hpp"
#include "AsepriteLayerChunk.hpp"
#include "AsepriteFrameHeader.hpp"
#include "AsepriteChunkHeader.hpp"
#include "AsepriteImageCelChunk.hpp"
#include "../../Physics/SIUnits.hpp"
#include "../../Cryptography/Compressor/ZLib.hpp"

namespace MathLib {
    bool Aseprite::Save(Writable& file) const {
        StartBenchmark
        AsepriteHeader header;
        header.frames = GetSize();
        header.width = GetWidth();
        header.height = GetHeight();
        header.bpp = 32;
        header.hasOpacityLayer = true;
        const AsepriteLayerChunk layer = AsepriteLayerChunk(header.width, header.height);
        const AsepriteImageCelChunk celChunk = AsepriteImageCelChunk(header.width, header.height, true);
        AsepriteFrameHeader frame;
        frame.size = sizeof(AsepriteFrameHeader) + layer.size + celChunk.size;
        frame.chunks16 = 2;
        header.size = sizeof(AsepriteHeader) + header.frames * frame.size;
        if (!file.Write<AsepriteHeader>(header)) ReturnFromBenchmark(false);
        for (uint32_t i = 0; i < header.frames; i++) {
            frame.duration = Millisecond<num_t>(Second<num_t>(frames.At(i).GetDuration())).GetValue();
            if (!file.Write<AsepriteFrameHeader>(frame) || !file.Write<AsepriteLayerChunk>(layer) || !file.Write<AsepriteImageCelChunk>(celChunk)) ReturnFromBenchmark(false);
            for (uint32_t y = 0; y < header.height; y++) {
                for (uint32_t x = 0; x < header.width; x++) {
                    const Color pixel = frames.At(i).pixels.At(x, y);
                    if (!file.Write<uint8_t>(pixel.rgba.r) || !file.Write<uint8_t>(pixel.rgba.g) || !file.Write<uint8_t>(pixel.rgba.b) || !file.Write<uint8_t>(pixel.rgba.a)) ReturnFromBenchmark(false);
                }
            }
        }
        ReturnFromBenchmark(true);
    }
    bool Aseprite::Load(Readable& file) {
        StartBenchmark
        AsepriteHeader header;
        if (!file.Read<AsepriteHeader>(header) || !header.IsValid() || header.bpp != 32) ReturnFromBenchmark(false);
        frames = Array<Frame>(header.frames);
        for (uint32_t i = 0; i < header.frames; i++) {
            AsepriteFrameHeader frame;
            if (!file.Read<AsepriteFrameHeader>(frame) || !frame.IsValid()) ReturnFromBenchmark(false);
            const uint32_t chunks = frame.chunks16 == UINT16_MAX && frame.chunks32 ? frame.chunks32 : frame.chunks16;
            for (uint64_t j = 0; j < chunks; j++) {
                uint32_t size = 0;
                if (!file.Read<uint32_t>(size) || size < sizeof(AsepriteChunkHeader)) ReturnFromBenchmark(false);
                uint8_t buff[size];
                MemoryCopy(&size, buff, sizeof(uint32_t));
                if (!file.ReadBuffer(buff + sizeof(uint32_t), size - sizeof(uint32_t))) ReturnFromBenchmark(false);
                const AsepriteChunkHeader* chunk = (const AsepriteChunkHeader*)buff;
                switch (chunk->type) {
                    case AsepriteChunkHeader::Type::Cel: {
                        const AsepriteCelChunk* cel = (const AsepriteCelChunk*)chunk;
                        if (cel->type == AsepriteCelChunk::Type::Raw || cel->type == AsepriteCelChunk::Type::CompressedImage) {
                            const AsepriteImageCelChunk* img = (const AsepriteImageCelChunk*)cel;
                            frames.At(i) = Frame(img->width, img->height, Millisecond<num_t>(frame.duration > 0 ? frame.duration : header.speed).ToBaseUnit().GetValue());
                            if (cel->type == AsepriteCelChunk::Type::CompressedImage) {
                                const Array<uint32_t> pixels = ZLib().DecryptT<uint32_t>(Array<uint8_t>((const uint8_t*)img->pixels, img->size - sizeof(AsepriteCelChunk)), MakeArray<uint64_t>(true));
                                for (size_t y = 0; y < img->height; y++) {
                                    for (size_t x = 0; x < img->width; x++) {
                                        const uint32_t pixel = pixels.At(y * img->width + x);
                                        const uint8_t* tmp = (const uint8_t*)&pixel;
                                        frames.At(i).pixels.At(x, y) = Color(tmp[0], tmp[1], tmp[2], tmp[3]).hex;
                                    }
                                }
                            }
                            else for (size_t y = 0; y < img->height; y++) {
                                for (size_t x = 0; x < img->width; x++) {
                                    const uint8_t* tmp = (const uint8_t*)&img->pixels[y * img->width + x];
                                    frames.At(i).pixels.At(x, y) = Color(tmp[0], tmp[1], tmp[2], tmp[3]).hex;
                                }
                            }
                        }
                    }
                    default: break;
                }
            }
        }
        ReturnFromBenchmark(true);
    }
}