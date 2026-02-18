#include "ZLib.hpp"
#include "Deflate.hpp"
#include "ZLibHeader.hpp"
#include "../OneWayCipher/Adler32.hpp"
#include "../../Interfaces/Sequence/ByteArray.hpp"
#include "../../Interfaces/Sequence/SubSequence.hpp"

namespace MathLib {
    Array<uint8_t> ZLib::Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const {
        if (!key.IsEmpty()) return Array<uint8_t>();
        ZLibHeader header;
        header.compressionMethod = (uint8_t)ZLibHeader::CompressionMethod::Deflate;
        header.level = (uint8_t)ZLibHeader::DeflateLevel::Fastest;
        for (uint8_t i = 0; i < 32; i++) {
            header.checkBits = i;
            if (!(SwapEndian16(*(const uint16_t*)&header) % 31)) break;
            if (i == 31) return Array<uint8_t>();
        }
        ByteArray file;
        if (!file.Write<ZLibHeader>(header)) return Array<uint8_t>();
        const Array<uint8_t> compressed = Deflate().Encrypt(data, CipherKey());
        return file.WriteBuffer(compressed.GetValue(), compressed.GetSize()) && file.Write<uint32_t>(SwapEndian32(Adler32().Get(data))) ? file.GetArray() : Array<uint8_t>();
    }
    Array<uint8_t> ZLib::DecryptReadablePartial(Readable& readable, const CipherKey& key, const Interval<size_t>& range) const {
        if (!(key.IsEmpty() || (key.type == CipherKey::Type::Normal && key.data.GetSize() == sizeof(bool)))) return Array<uint8_t>();
        const bool noSignatureCheck = !key.IsEmpty() && key.data.AsT<bool>().Get();
        ZLibHeader header;
        if (!readable.Read<ZLibHeader>(header) || header.compressionMethod != (uint8_t)ZLibHeader::CompressionMethod::Deflate || header.dictionary) return Array<uint8_t>();
        if (noSignatureCheck) return Deflate().DecryptReadablePartial(readable, CipherKey(), range);
        const Array<uint8_t> ret = Deflate().DecryptReadable(readable, CipherKey());
        uint32_t adler32 = 0;
        if (!readable.Read<uint32_t>(adler32) || Adler32().Get(ret) != SwapEndian32(adler32)) return Array<uint8_t>();
        const size_t start = range.GetMin();
        const size_t end = Min<size_t>(range.GetMax(), ret.GetSize());
        return start < end ? CollectionToArray<uint8_t>(SubSequence<uint8_t>(ret, Interval<size_t>(start, end))) : Array<uint8_t>();
    }
}