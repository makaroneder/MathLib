#include "ZLib.hpp"
#include "Deflate.hpp"
#include "ZLibHeader.hpp"
#include "../OneWayCipher/Adler32.hpp"
#include "../../Interfaces/ByteArray.hpp"
#include "../../Interfaces/SubSequence.hpp"

namespace MathLib {
    Array<uint8_t> ZLib::Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key, bool encrypt) const {
        StartBenchmark
        if (!key.IsEmpty()) ReturnFromBenchmark(Array<uint8_t>());
        ZLibHeader header;
        if (encrypt) {
            ByteArray file;
            header.compressionMethod = (uint8_t)ZLibHeader::CompressionMethod::Deflate;
            header.level = (uint8_t)ZLibHeader::DeflateLevel::Fastest;
            for (uint8_t i = 0; i < 32; i++) {
                header.checkBits = i;
                if (!(SwapEndian16(*(const uint16_t*)&header) % 31)) break;
                if (i == 31) ReturnFromBenchmark(Array<uint8_t>());
            }
            if (!file.Write<ZLibHeader>(header)) ReturnFromBenchmark(Array<uint8_t>());
            const Array<uint8_t> compressed = Deflate().Encrypt(data, Array<uint64_t>(), true);
            ReturnFromBenchmark(file.WriteBuffer(compressed.GetValue(), compressed.GetSize()) && file.Write<uint32_t>(SwapEndian32(Adler32().Get(data))) ? file.GetArray() : Array<uint8_t>());
        }
        ByteArray file = ByteArray(data);
        if (!file.Read<ZLibHeader>(header) || header.compressionMethod != (uint8_t)ZLibHeader::CompressionMethod::Deflate || header.dictionary) ReturnFromBenchmark(Array<uint8_t>());
        const size_t offset = sizeof(uint32_t);
        const Array<uint8_t> ret = Deflate().Encrypt(SubSequence<uint8_t>(data, Interval<size_t>(file.Tell(), file.GetSize() - offset)), Array<uint64_t>(), false);
        if (!file.Seek(offset, SeekMode::End)) ReturnFromBenchmark(Array<uint8_t>());
        uint32_t adler32;
        ReturnFromBenchmark(file.Read<uint32_t>(adler32) && Adler32().Get(ret) == SwapEndian32(adler32) ? ret : Array<uint8_t>());
    }
}