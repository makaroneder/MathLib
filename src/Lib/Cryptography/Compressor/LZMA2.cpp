#include "LZMA2.hpp"
#include "LZMA2ChunkHeader.hpp"
#include "../../Interfaces/Sequence/SubSequence.hpp"

namespace MathLib {
    size_t LZMA2::GetDictionarySize(uint8_t key) const {
        if (key > 40) return SIZE_MAX;
        if (key == 40) return UINT32_MAX;
        return ((key & 1) | 0b10) << (key / 2 + 11);
    }
    Array<uint8_t> LZMA2::Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const {
        if (key.type != CipherKey::Type::Normal || key.data.GetSize() != sizeof(uint8_t)) return Array<uint8_t>();
        const size_t dictionarySize = GetDictionarySize(key.data.AsT<uint8_t>().Get());
        if (dictionarySize == SIZE_MAX) return Array<uint8_t>();
        ByteArray ret;
        size_t size = data.GetSize();
        size_t i = 0;
        while (true) {
            if (!ret.Write<uint8_t>(0x02)) return Array<uint8_t>();
            uint16_t writeSize = UINT16_MAX;
            bool last = false;
            if (size <= UINT16_MAX) {
                last = true;
                writeSize = size;
            }
            size -= writeSize;
            if (!ret.Write<uint16_t>(SwapEndian16(size - 1))) return Array<uint8_t>();
            for (uint32_t j = 0; j < size; j++)
                if (!ret.Write<uint8_t>(data.AtUnsafe(i++))) return Array<uint8_t>();
            if (!last) continue;
            if (!ret.Write<uint8_t>(0x00)) return Array<uint8_t>();
            break;
        }
        return ret.GetArray();
    }
    Array<uint8_t> LZMA2::DecryptReadablePartial(Readable& readable, const CipherKey& key, const Interval<size_t>& range) const {
        if (key.type != CipherKey::Type::Normal || key.data.GetSize() != sizeof(uint8_t)) return Array<uint8_t>();
        const size_t dictionarySize = GetDictionarySize(key.data.AsT<uint8_t>().Get());
        if (dictionarySize == SIZE_MAX) return Array<uint8_t>();
        Array<uint8_t> ret;
        while (true) {
            uint8_t controlByte;
            if (!readable.Read<uint8_t>(controlByte)) return Array<uint8_t>();
            if (!controlByte) break;
            switch (controlByte) {
                case 0x01:
                case 0x02: {
                    if (controlByte == 0x01) {
                        // TODO: Reset dictionary
                    }
                    uint16_t size;
                    if (!readable.Read<uint16_t>(size)) return Array<uint8_t>();
                    size = SwapEndian16(size);
                    for (uint32_t i = 0; i <= size; i++) {
                        uint8_t tmp;
                        if (!readable.Read<uint8_t>(tmp) || !ret.Add(tmp)) return Array<uint8_t>();
                    }
                    break;
                }
                case 0x80 ... 0xff: {
                    const LZMA2ChunkHeader header = *(const LZMA2ChunkHeader*)&controlByte;
                    uint16_t lowUncompressedSize;
                    if (!readable.Read<uint16_t>(lowUncompressedSize)) return Array<uint8_t>();
                    lowUncompressedSize = SwapEndian16(lowUncompressedSize) + 1;
                    uint16_t compressedSize;
                    if (!readable.Read<uint16_t>(compressedSize)) return Array<uint8_t>();
                    compressedSize = SwapEndian16(compressedSize) + 1;
                    if (header.reset >= (uint8_t)LZMA2ChunkHeader::ResetType::StateAndProperties) {
                        uint8_t properties;
                        if (!readable.Read<uint8_t>(properties)) return Array<uint8_t>();
                    }
                    uint8_t buff[compressedSize];
                    if (!readable.ReadBuffer(buff, compressedSize)) return Array<uint8_t>();
                    // TODO: Decompress
                    return Array<uint8_t>();
                }
                default: return Array<uint8_t>();
            }
        }
        const size_t start = range.GetMin();
        const size_t end = Min<size_t>(range.GetMax(), ret.GetSize());
        return start < end ? CollectionToArray<uint8_t>(SubSequence<uint8_t>(ret, Interval<size_t>(start, end))) : Array<uint8_t>();
    }
}