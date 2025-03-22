#include "GZIP.hpp"
#include "Deflate.hpp"
#include "GZIPHeader.hpp"
#include "../OneWayCipher/CRC.hpp"
#include "../../Interfaces/ByteArray.hpp"
#include "../../Interfaces/SubSequence.hpp"

namespace MathLib {
    Array<uint8_t> GZIP::Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key, bool encrypt) const {
        StartBenchmark
        if (!key.IsEmpty()) ReturnFromBenchmark(Array<uint8_t>());
        // TODO: Add compression
        if (encrypt) ReturnFromBenchmark(Array<uint8_t>());
        ByteArray file = ByteArray(data);
        GZIPHeader header;
        if (!file.Read<GZIPHeader>(header) || !header.IsValid() || header.compressionMethod != GZIPHeader::CompressionMethod::Deflate) ReturnFromBenchmark(Array<uint8_t>());
        if (header.extraData) {
            uint16_t size;
            if (!file.Read<uint16_t>(size)) ReturnFromBenchmark(Array<uint8_t>());
            uint8_t buff[size];
            if (!file.ReadBuffer(buff, size)) ReturnFromBenchmark(Array<uint8_t>());
        }
        for (uint8_t i = 0; i < header.name + header.comment; i++)
            if (file.ReadUntil('\0').IsEmpty()) ReturnFromBenchmark(Array<uint8_t>());
        if (header.crc16) {
            uint16_t crc16;
            if (!file.Read<uint16_t>(crc16)) ReturnFromBenchmark(Array<uint8_t>());
            // TODO: Check CRC16
        }
        const size_t offset = 2 * sizeof(uint32_t);
        const Array<uint8_t> ret = Deflate().Encrypt(SubSequence<uint8_t>(data, Interval<size_t>(file.Tell(), file.GetSize() - offset)), Array<uint64_t>(), false);
        if (!file.Seek(offset, SeekMode::End)) ReturnFromBenchmark(Array<uint8_t>());
        uint32_t crc;
        if (!file.Read<uint32_t>(crc)) ReturnFromBenchmark(Array<uint8_t>());
        uint32_t expectedSize;
        if (!file.Read<uint32_t>(expectedSize) || expectedSize != (uint32_t)ret.GetSize()) ReturnFromBenchmark(Array<uint8_t>());
        // TODO: Check CRC32
        ReturnFromBenchmark(ret);
    }
}