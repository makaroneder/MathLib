#include "GZIP.hpp"
#include "Deflate.hpp"
#include "GZIPHeader.hpp"
#include "../../ExternArray.hpp"
#include "../OneWayCipher/CRC.hpp"
#include "../../Interfaces/Sequence/ByteArray.hpp"
#include "../../Interfaces/Sequence/SubSequence.hpp"

namespace MathLib {
    Array<uint8_t> GZIP::Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key) const {
        if (key.GetSize() > 1) return Array<uint8_t>();
        const GZIPHeader header = GZIPHeader(false, false, false, false, false, GZIPHeader::ExtraFlags::FastestCompression, GZIPHeader::OperatingSystem::Unix);
        ByteArray byteArray;
        if (!byteArray.Write<GZIPHeader>(header)) return Array<uint8_t>();
        if (header.extraData && (!byteArray.Write<uint16_t>(2) || !byteArray.Write<uint16_t>(0x2137))) return Array<uint8_t>();
        const Array<uint64_t> crcKey = MakeArray<uint64_t>(32, (uint64_t)CRC::Polynomial::CRC32, true, true, UINT32_MAX, true);
        if (header.crc16 && !byteArray.Write<uint16_t>(*(const uint16_t*)CRC().Encrypt(byteArray, crcKey).GetValue())) return Array<uint8_t>();
        if (!byteArray.AddSequence(Deflate().Encrypt(data, Array<uint64_t>())) || !byteArray.AddSequence(CRC().Encrypt(data, crcKey))) return Array<uint8_t>();
        const uint32_t size = data.GetSize();
        if (!byteArray.AddSequence(ExternArray<uint8_t>((uint8_t*)&size, sizeof(uint32_t)))) return Array<uint8_t>();
        return byteArray.GetArray();
    }
    Array<uint8_t> GZIP::DecryptReadablePartial(Readable& readable, const Sequence<uint64_t>& key, const Interval<size_t>& range) const {
        if (key.GetSize() > 1) return Array<uint8_t>();
        GZIPHeader header;
        if (!readable.Read<GZIPHeader>(header) || !header.IsValid() || header.compressionMethod != GZIPHeader::CompressionMethod::Deflate) return Array<uint8_t>();
        Array<uint8_t> headerData = Array<uint8_t>((const uint8_t*)&header, sizeof(GZIPHeader));
        if (header.extraData) {
            uint16_t size;
            if (!readable.Read<uint16_t>(size)) return Array<uint8_t>();
            const uint8_t* tmp = (const uint8_t*)&size;
            if (!headerData.Add(tmp[0]) || !headerData.Add(tmp[1])) return Array<uint8_t>();
            for (uint32_t i = 0; i < size; i++) {
                uint8_t tmp;
                if (!readable.Read(tmp) || !headerData.Add(tmp)) return Array<uint8_t>();
            }
        }
        for (uint8_t i = 0; i < header.name + header.comment; i++) {
            while (true) {
                uint8_t tmp;
                if (!readable.Read<uint8_t>(tmp) || !headerData.Add(tmp)) return Array<uint8_t>();
                if (!tmp) break;
            }
        }
        const Array<uint64_t> crcKey = MakeArray<uint64_t>(32, (uint64_t)CRC::Polynomial::CRC32, true, true, UINT32_MAX, true);
        if (header.crc16) {
            uint16_t crc16;
            if (!readable.Read<uint16_t>(crc16)) return Array<uint8_t>();
            if (*(const uint16_t*)CRC().Encrypt(headerData, crcKey).GetValue() != crc16) return Array<uint8_t>();
        }
        if (!key.IsEmpty() && key.At(0)) return Deflate().DecryptReadablePartial(readable, Array<uint64_t>(), range);
        const Array<uint8_t> ret = Deflate().DecryptReadable(readable, Array<uint64_t>());
        uint32_t crc;
        if (!readable.Read<uint32_t>(crc)) return Array<uint8_t>();
        uint32_t expectedSize;
        if (!readable.Read<uint32_t>(expectedSize) || expectedSize != (uint32_t)ret.GetSize()) return Array<uint8_t>();
        if (!CRC().IsEncrypted<uint8_t>(ret, Array<uint8_t>((const uint8_t*)&crc, sizeof(uint32_t)), crcKey)) return Array<uint8_t>();
        const size_t start = range.GetMin();
        const size_t end = Min<size_t>(range.GetMax(), ret.GetSize());
        return start < end ? CollectionToArray<uint8_t>(SubSequence<uint8_t>(ret, Interval<size_t>(start, end))) : Array<uint8_t>();
    }
}