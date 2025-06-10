#include "ZLib.hpp"
#include "Deflate.hpp"
#include "ZLibHeader.hpp"
#include "../OneWayCipher/Adler32.hpp"
#include "../../Interfaces/ByteArray.hpp"
#include "../../Interfaces/SubSequence.hpp"

namespace MathLib {
    Array<uint8_t> ZLib::Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key) const {
        StartBenchmark
        if (!key.IsEmpty()) ReturnFromBenchmark(Array<uint8_t>());
        ZLibHeader header;
        header.compressionMethod = (uint8_t)ZLibHeader::CompressionMethod::Deflate;
        header.level = (uint8_t)ZLibHeader::DeflateLevel::Fastest;
        for (uint8_t i = 0; i < 32; i++) {
            header.checkBits = i;
            if (!(SwapEndian16(*(const uint16_t*)&header) % 31)) break;
            if (i == 31) ReturnFromBenchmark(Array<uint8_t>());
        }
        ByteArray file;
        if (!file.Write<ZLibHeader>(header)) ReturnFromBenchmark(Array<uint8_t>());
        const Array<uint8_t> compressed = Deflate().Encrypt(data, Array<uint64_t>());
        ReturnFromBenchmark(file.WriteBuffer(compressed.GetValue(), compressed.GetSize()) && file.Write<uint32_t>(SwapEndian32(Adler32().Get(data))) ? file.GetArray() : Array<uint8_t>());
    }
    Array<uint8_t> ZLib::DecryptReadablePartial(Readable& readable, const Sequence<uint64_t>& key, const Interval<size_t>& range) const {
        StartBenchmark
        if (key.GetSize() > 1) ReturnFromBenchmark(Array<uint8_t>());
        ZLibHeader header;
        if (!readable.Read<ZLibHeader>(header) || header.compressionMethod != (uint8_t)ZLibHeader::CompressionMethod::Deflate || header.dictionary) ReturnFromBenchmark(Array<uint8_t>());
        if (key.IsEmpty() || !key.At(0)) {
            const Array<uint8_t> ret = Deflate().DecryptReadable(readable, Array<uint64_t>());
            uint32_t adler32 = 0;
            if (!readable.Read<uint32_t>(adler32) || Adler32().Get(ret) != SwapEndian32(adler32)) ReturnFromBenchmark(Array<uint8_t>());
            const size_t start = range.GetMin();
            const size_t end = Min<size_t>(range.GetMax(), ret.GetSize());
            ReturnFromBenchmark(start < end ? CollectionToArray<uint8_t>(SubSequence<uint8_t>(ret, Interval<size_t>(start, end))) : Array<uint8_t>());
        }
        ReturnFromBenchmark(Deflate().DecryptReadablePartial(readable, Array<uint64_t>(), range));
    }
}