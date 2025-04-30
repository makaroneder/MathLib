#include "PNG.hpp"
#include "PNGHeader.hpp"
#include "IENDChunk.hpp"
#include "IHDRChunk.hpp"
#include "IDATChunk.hpp"
#include "../../Color.hpp"
#include "../../Memory.hpp"
#include "PNGFilterMethod.hpp"
#include "../../ExternArray.hpp"
#include "../../Cryptography/Compressor/ZLib.hpp"
#include "../../Cryptography/OneWayCipher/CRC.hpp"

namespace MathLib {
    PNG::PNG(size_t width, size_t height) : SaveableImage(width, height) {
        EmptyBenchmark
    }
    bool PNG::Save(Writable& file) const {
        StartBenchmark
        const size_t width = GetWidth();
        const size_t height = GetHeight();
        const Array<uint64_t> crcKey = MakeArray<uint64_t>(32, (uint64_t)CRC::Polynomial::CRC32, true, true, UINT32_MAX, true);
        const CRC crc;
        PNGHeader header;
        if (!file.Write<PNGHeader>(header)) ReturnFromBenchmark(false);
        const IHDRChunk ihdr = IHDRChunk(width, height, 8, IHDRChunk::ColorType::RGBA, IHDRChunk::CompressionMethod::Deflate, IHDRChunk::FilterMethod::None, IHDRChunk::InterlaceMethod::None);
        if (!file.Write<IHDRChunk>(ihdr)) ReturnFromBenchmark(false);
        if (!file.Write<uint32_t>(SwapEndian32(*(const uint32_t*)crc.Encrypt(ExternArray<uint8_t>((uint8_t*)&ihdr, sizeof(IHDRChunk)), crcKey).GetValue()))) ReturnFromBenchmark(false);
        const size_t size = height * (width * sizeof(uint32_t) + 1);
        const IDATChunk idat = IDATChunk(size);
        if (!file.Write<IDATChunk>(idat)) ReturnFromBenchmark(false);
        Array<uint8_t> data = Array<uint8_t>(size);
        for (uint32_t y = 0; y < height; y++) {
            const size_t baseY = y * (width * sizeof(uint32_t) + 1);
            data.At(baseY) = (uint8_t)PNGFilterMethod::None;
            for (uint32_t x = 0; x < width; x++) {
                const uint32_t baseX = baseY + x * sizeof(uint32_t) + 1;
                const uint32_t pixel = At(x, y);
                const uint8_t* buff = (const uint8_t*)&pixel;
                for (uint8_t i = 0; i < sizeof(uint32_t); i++) data.At(baseX + i) = buff[i];
            }
        }
        switch (ihdr.compressionMethod) {
            case IHDRChunk::CompressionMethod::Deflate: {
                data = ZLib().Encrypt(data, MathLib::Array<uint64_t>());
                break;
            }
            default: ReturnFromBenchmark(false);
        }
        if (!file.WriteBuffer(data.GetValue(), data.GetSize())) ReturnFromBenchmark(false);
        Array<uint8_t> tmp = Array<uint8_t>((uint8_t*)&idat, sizeof(IDATChunk));
        if (!tmp.AddSequence(data)) ReturnFromBenchmark(false);
        if (!file.Write<uint32_t>(UINT32_MAX)) ReturnFromBenchmark(false);
        // if (!file.Write<uint32_t>(SwapEndian32(*(const uint32_t*)crc.Encrypt(tmp, crcKey).GetValue()))) ReturnFromBenchmark(false);
        const IENDChunk iend;
        if (!file.Write<IENDChunk>(iend)) ReturnFromBenchmark(false);
        if (!file.Write<uint32_t>(SwapEndian32(*(const uint32_t*)crc.Encrypt(ExternArray<uint8_t>((uint8_t*)&iend, sizeof(IENDChunk)), crcKey).GetValue()))) ReturnFromBenchmark(false);
        ReturnFromBenchmark(true);
    }
    bool PNG::Load(Readable& file) {
        StartBenchmark
        PNGHeader header;
        if (!file.Read<PNGHeader>(header) || !header.IsValid()) ReturnFromBenchmark(false);
        uint8_t bpp;
        Array<uint8_t> data;
        while (true) {
            uint32_t size;
            if (!file.Read<uint32_t>(size)) ReturnFromBenchmark(false);
            size = SwapEndian32(size);
            uint8_t buff[size + sizeof(PNGChunk)];
            MemoryCopy(&size, buff, sizeof(uint32_t));
            if (!file.ReadBuffer(buff + sizeof(uint32_t), SizeOfArray(buff) - sizeof(uint32_t))) ReturnFromBenchmark(false);
            const PNGChunk* chunk = (const PNGChunk*)buff;
            uint32_t crc32;
            if (!file.Read<uint32_t>(crc32)) ReturnFromBenchmark(false);
            crc32 = SwapEndian32(crc32);
            // TODO: Check CRC32
            if (!chunk->IsValid()) ReturnFromBenchmark(false);
            const uint32_t type = *(const uint32_t*)chunk->type;
            if (type == *(const uint32_t*)IENDChunk::expectedType) break;
            else if (type == *(const uint32_t*)IHDRChunk::expectedType) {
                const IHDRChunk* ihdr = (const IHDRChunk*)chunk;
                pixels = Matrix<uint32_t>(SwapEndian32(ihdr->width), SwapEndian32(ihdr->height));
                if (ihdr->bitDepth != 8) ReturnFromBenchmark(false);
                if (ihdr->colorType == IHDRChunk::ColorType::RGB) bpp = 3;
                else if (ihdr->colorType == IHDRChunk::ColorType::RGBA) bpp = 4;
                else ReturnFromBenchmark(false);
                if (ihdr->compressionMethod != IHDRChunk::CompressionMethod::Deflate) ReturnFromBenchmark(false);
                if (ihdr->filterMethod != IHDRChunk::FilterMethod::None) ReturnFromBenchmark(false);
                if (ihdr->interlaceMethod != IHDRChunk::InterlaceMethod::None) ReturnFromBenchmark(false);
            }
            else if (type == *(const uint32_t*)IDATChunk::expectedType) {
                const IDATChunk* idat = (const IDATChunk*)chunk;
                data += Array<uint8_t>(idat->data, idat->length);
            }
            else if (chunk->IsCritical()) ReturnFromBenchmark(false);
        }
        const Array<uint8_t> tmp = ZLib().Decrypt(data, MathLib::Array<uint64_t>());
        Array<PNGFilterMethod> filters = Array<PNGFilterMethod>(GetHeight());
        Matrix<uint32_t> raw = Matrix<uint32_t>(GetWidth(), GetHeight());
        for (size_t y = 0; y < GetHeight(); y++) {
            const size_t baseY = (bpp * GetWidth() + 1) * y;
            filters.At(y) = (PNGFilterMethod)tmp.At(baseY);
            for (size_t x = 0; x < GetWidth(); x++) {
                const size_t baseX = baseY + x * bpp + 1;
                uint8_t buff[4] = { 0, 0, 0, 0xff, };
                for (uint8_t i = 0; i < bpp; i++) buff[i] = tmp.At(baseX + i);
                raw.At(x, y) = *(const uint32_t*)buff;
            }
        }
        for (size_t y = 0; y < GetHeight(); y++) {
            for (size_t x = 0; x < GetWidth(); x++) {
                switch (filters.At(y)) {
                    case PNGFilterMethod::None: break;
                    case PNGFilterMethod::Sub: {
                        uint8_t buff[bpp];
                        *(uint32_t*)buff = raw.At(x, y);
                        if (x) {
                            const uint8_t* buffX = (const uint8_t*)&raw.At(x - 1, y);
                            for (uint8_t i = 0; i < bpp; i++) buff[i] += buffX[i];
                        }
                        raw.At(x, y) = *(const uint32_t*)buff;
                        break;
                    }
                    case PNGFilterMethod::Up: {
                        uint8_t buff[bpp];
                        *(uint32_t*)buff = raw.At(x, y);
                        if (y) {
                            const uint8_t* buffY = (const uint8_t*)&raw.At(x, y - 1);
                            for (uint8_t i = 0; i < bpp; i++) buff[i] += buffY[i];
                        }
                        raw.At(x, y) = *(const uint32_t*)buff;
                        break;
                    }
                    case PNGFilterMethod::Average: {
                        uint8_t buff[bpp];
                        uint8_t buffX[bpp];
                        uint8_t buffY[bpp];
                        *(uint32_t*)buff = raw.At(x, y);
                        *(uint32_t*)buffX = *(uint32_t*)buffY = 0;
                        if (x) *(uint32_t*)buffX = raw.At(x - 1, y);
                        if (y) *(uint32_t*)buffY = raw.At(x, y - 1);
                        for (uint8_t i = 0; i < bpp; i++) buff[i] += (buffX[i] + buffY[i]) / 2;
                        raw.At(x, y) = *(const uint32_t*)buff;
                        break;
                    }
                    case PNGFilterMethod::Paeth: {
                        uint8_t buff[bpp];
                        uint8_t buffX[bpp];
                        uint8_t buffY[bpp];
                        uint8_t buffXY[bpp];
                        *(uint32_t*)buff = raw.At(x, y);
                        *(uint32_t*)buffX = *(uint32_t*)buffY = *(uint32_t*)buffXY = 0;
                        if (x) {
                            *(uint32_t*)buffX = raw.At(x - 1, y);
                            if (y) *(uint32_t*)buffXY = raw.At(x - 1, y - 1);
                        }
                        if (y) *(uint32_t*)buffY = raw.At(x, y - 1);
                        for (uint8_t i = 0; i < bpp; i++) {
                            const ssize_t p = (ssize_t)buffX[i] + (ssize_t)buffY[i] - (ssize_t)buffXY[i];
                            const size_t pX = Abs(p - buffX[i]);
                            const size_t pY = Abs(p - buffY[i]);
                            const size_t pXY = Abs(p - buffXY[i]);
                            if (pX <= pY && pX <= pXY) buff[i] += buffX[i];
                            else if (pY <= pXY) buff[i] += buffY[i];
                            else buff[i] += buffXY[i];
                        }
                        raw.At(x, y) = *(const uint32_t*)buff;
                        break;
                    }
                    default: ReturnFromBenchmark(false);
                }
                const uint8_t* buff = (const uint8_t*)&raw.At(x, y);
                pixels.At(x, y) = Color(buff[0], buff[1], buff[2], 0xff).hex;
            }
        }

        ReturnFromBenchmark(true);
    }
}