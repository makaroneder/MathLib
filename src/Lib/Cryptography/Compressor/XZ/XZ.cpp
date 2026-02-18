#include "XZ.hpp"
#include "XZHeader.hpp"
#include "XZFooter.hpp"
#include "XZFilter.hpp"
#include "../LZMA2.hpp"
#include "XZBlockFlags.hpp"
#include "../../OneWayCipher/CRC.hpp"
#include "../../../Interfaces/Sequence/SubSequence.hpp"

namespace MathLib {
    Array<uint8_t> XZ::Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const {
        // TODO:
        (void)data;
        (void)key;
        return Array<uint8_t>();
    }
    Array<uint8_t> XZ::DecryptPartial(const Sequence<uint8_t>& data, const CipherKey& key, const Interval<size_t>& range) const {
        if (!key.IsEmpty()) return Array<uint8_t>();
        const CipherKey crcKey = CipherKey(MakeArray<CipherKey>(
            CipherKey(ByteArray::ToByteArray<size_t>(MakeArray<size_t>(32))),
            CipherKey(ByteArray::ToByteArray<CRC::Polynomial>(MakeArray<CRC::Polynomial>(CRC::Polynomial::CRC32))),
            CipherKey(MakeArray<uint8_t>(true)),
            CipherKey(MakeArray<uint8_t>(true)),
            CipherKey(ByteArray::ToByteArray<uint64_t>(MakeArray<uint64_t>(UINT32_MAX))),
            CipherKey(MakeArray<uint8_t>(true))
        ));
        ByteArray readable = data;
        XZHeader header;
        if (!readable.Read<XZHeader>(header) || !header.IsValid()) return Array<uint8_t>();
        const size_t position = readable.Tell();
        const uint8_t checkSize = header.flags.GetSizeOfCheck();
        if (!readable.Seek(sizeof(uint16_t) * 2 + sizeof(uint32_t) * 2, SeekMode::End)) return Array<uint8_t>();
        XZFooter footer;
        if (!readable.Read<XZFooter>(footer) || !footer.IsValid() || footer.flags != header.flags) return Array<uint8_t>();
        if (!readable.Seek(sizeof(uint16_t) * 2 + sizeof(uint32_t) * 2 + footer.GetIndexSize(), SeekMode::End)) return Array<uint8_t>();
        const Array<XZRecord> records = ParseIndexStructure(readable);
        if (!readable.Seek(position, SeekMode::Set)) return Array<uint8_t>();
        Array<uint8_t> ret;
        for (const XZRecord& record : records) {
            uint8_t headerSize;
            if (!readable.Read<uint8_t>(headerSize)) return Array<uint8_t>();
            if (!headerSize) break;
            XZBlockFlags flags;
            if (!readable.Read<XZBlockFlags>(flags)) return Array<uint8_t>();
            Array<uint8_t> array = MakeArray<uint8_t>(headerSize, *(const uint8_t*)&flags);
            const size_t compressedSize = flags.hasCompressedSize ? GetVariableLengthInteger(readable, array) : 0;
            if (compressedSize == SIZE_MAX) return Array<uint8_t>();
            const size_t uncompressedSize = flags.hasUncompressedSize ? GetVariableLengthInteger(readable, array) : 0;
            if (uncompressedSize == SIZE_MAX) return Array<uint8_t>();
            Array<XZFilter> filters = flags.filters + 1;
            for (uint8_t i = 0; i <= flags.filters; i++) {
                const size_t id = GetVariableLengthInteger(readable, array);
                if (id == SIZE_MAX) return Array<uint8_t>();
                const size_t size = GetVariableLengthInteger(readable, array);
                if (size == SIZE_MAX) return Array<uint8_t>();
                Array<uint8_t> filterData = size;
                for (size_t j = 0; j < size; j++) {
                    uint8_t tmp;
                    if (!readable.Read<uint8_t>(tmp) || !array.Add(tmp)) return Array<uint8_t>();
                    filterData.AtUnsafe(j) = tmp;    
                }
                filters.AtUnsafe(i) = XZFilter((XZFilter::Type)id, filterData);
            }
            const uint16_t realHeaderSize = (headerSize + 1) * 4;
            const uint16_t paddingSize = realHeaderSize - array.GetSize() - sizeof(uint32_t);
            for (uint16_t i = 0; i < paddingSize; i++) {
                uint8_t tmp;
                if (!readable.Read<uint8_t>(tmp) || tmp || !array.Add(tmp)) return Array<uint8_t>();
            }
            uint32_t crc32;
            if (!readable.Read<uint32_t>(crc32)) return Array<uint8_t>();
            const Array<uint8_t> tmp = CRC().Encrypt(array, crcKey);
            const size_t start = array.GetSize() + checkSize + sizeof(uint32_t);
            if (tmp.GetSize() != sizeof(uint32_t) || crc32 != *(const uint32_t*)tmp.GetValue() || !array.Reset()) return Array<uint8_t>();
            for (uint32_t i = start; i < record.unpaddedSize; i++) {
                uint8_t tmp;
                if (!readable.Read<uint8_t>(tmp) || !array.Add(tmp)) return Array<uint8_t>();
            }
            for (uint8_t i = 0; (array.GetSize() + i) % 4; i++) {
                uint8_t tmp;
                if (!readable.Read<uint8_t>(tmp)) return Array<uint8_t>();
            }
            uint8_t expectedCheck[checkSize];
            if (!readable.ReadBuffer(expectedCheck, checkSize)) return Array<uint8_t>();
            for (const XZFilter& filter : filters) {
                switch (filter.id) {
                    case XZFilter::Type::LZMA2: {
                        if (filter.data.GetSize() != 1) return Array<uint8_t>();
                        array = LZMA2().Decrypt(array, CipherKey(MakeArray<uint8_t>(filter.data.AtUnsafe(0) & 0x3f)));
                        break;
                    }
                    default: return Array<uint8_t>();
                }
            }
            Array<uint8_t> check;
            switch ((XZFlags::CheckType)header.flags.checkType) {
                case XZFlags::CheckType::None: break;
                case XZFlags::CheckType::CRC32: {
                    check = CRC().Encrypt(array, crcKey);
                    break;
                }
                case XZFlags::CheckType::CRC64: {
                    const CipherKey crc64Key = CipherKey(MakeArray<CipherKey>(
                        CipherKey(ByteArray::ToByteArray<size_t>(MakeArray<size_t>(64))),
                        CipherKey(ByteArray::ToByteArray<CRC::Polynomial>(MakeArray<CRC::Polynomial>(CRC::Polynomial::CRC64ECMA))),
                        CipherKey(MakeArray<uint8_t>(true)),
                        CipherKey(MakeArray<uint8_t>(true)),
                        CipherKey(ByteArray::ToByteArray<uint64_t>(MakeArray<uint64_t>(UINT64_MAX))),
                        CipherKey(MakeArray<uint8_t>(true))
                    ));
                    check = CRC().Encrypt(array, crc64Key);
                    break;
                }
                default: return Array<uint8_t>();
            }
            if (check != ExternArray<uint8_t>(expectedCheck, checkSize)) return Array<uint8_t>();
            if (!ret.AddSequence(array)) return Array<uint8_t>();
        }
        const size_t start = range.GetMin();
        const size_t end = Min<size_t>(range.GetMax(), ret.GetSize());
        return start < end ? CollectionToArray<uint8_t>(SubSequence<uint8_t>(ret, Interval<size_t>(start, end))) : Array<uint8_t>();
    }
    size_t XZ::GetVariableLengthInteger(Readable& readable, WritableSequence<uint8_t>& array) const {
        uint8_t prev;
        if (!readable.Read(prev) || !array.Add(prev)) return SIZE_MAX;
        size_t ret = prev & 0x7f;
        size_t i = 0;
        while (prev & 0x80) {
            if (!readable.Read(prev) || !prev || !array.Add(prev)) return SIZE_MAX;
            ret |= (uint64_t)(prev & 0x7f) << (++i * 7);
        }
        return ret;
    }
    Array<XZRecord> XZ::ParseIndexStructure(Readable& readable) const {
        uint8_t zero;
        if (!readable.Read<uint8_t>(zero) || zero) return Array<XZRecord>();
        Array<uint8_t> arr = MakeArray<uint8_t>(zero);
        const size_t recordCount = GetVariableLengthInteger(readable, arr);
        Array<XZRecord> records = recordCount;
        for (size_t i = 0; i < recordCount; i++) {
            const size_t unpaddedSize = GetVariableLengthInteger(readable, arr);
            records.AtUnsafe(i) = XZRecord(unpaddedSize, GetVariableLengthInteger(readable, arr));
        }
        while (arr.GetSize() % 4) {
            uint8_t zero;
            if (!readable.Read<uint8_t>(zero) || zero || !arr.Add(zero)) return Array<XZRecord>();
        }
        uint32_t crc32;
        if (!readable.Read<uint32_t>(crc32)) return Array<XZRecord>();
        const CipherKey crcKey = CipherKey(MakeArray<CipherKey>(
            CipherKey(ByteArray::ToByteArray<size_t>(MakeArray<size_t>(32))),
            CipherKey(ByteArray::ToByteArray<CRC::Polynomial>(MakeArray<CRC::Polynomial>(CRC::Polynomial::CRC32))),
            CipherKey(MakeArray<uint8_t>(true)),
            CipherKey(MakeArray<uint8_t>(true)),
            CipherKey(ByteArray::ToByteArray<uint64_t>(MakeArray<uint64_t>(UINT32_MAX))),
            CipherKey(MakeArray<uint8_t>(true))
        ));
        const Array<uint8_t> crc = CRC().Encrypt(arr, crcKey);
        return crc.GetSize() == sizeof(uint32_t) && crc32 == *(const uint32_t*)crc.GetValue() ? records : Array<XZRecord>();
    }
}