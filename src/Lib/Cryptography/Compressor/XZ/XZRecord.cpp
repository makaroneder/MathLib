#include "XZRecord.hpp"
#include "XZFilter.hpp"
#include "XZCRC32Key.hpp"
#include "XZBlockFlags.hpp"
#include "XZVariableLengthInteger.hpp"
#include "../../../Interfaces/SavedReadable.hpp"
#include "../../../Interfaces/Sequence/SubSequence.hpp"

namespace MathLib {
    XZRecord::XZRecord(void) : unpaddedSize(), uncompressedSize() {}
    XZRecord::XZRecord(size_t unpaddedSize, size_t uncompressedSize) : unpaddedSize(unpaddedSize), uncompressedSize(uncompressedSize) {}
    bool XZRecord::Equals(const XZRecord& other) const {
        return unpaddedSize == other.unpaddedSize && uncompressedSize == other.uncompressedSize;
    }
    bool XZRecord::Save(Writable& file) const {
        const XZVariableLengthInteger integer;
        return integer.EncryptWritable(file, Array<uint8_t>((const uint8_t*)&unpaddedSize, sizeof(size_t)), CipherKey()) && integer.EncryptWritable(file, Array<uint8_t>((const uint8_t*)&uncompressedSize, sizeof(size_t)), CipherKey());
    }
    bool XZRecord::Load(Readable& file) {
        const XZVariableLengthInteger integer;
        unpaddedSize = integer.Get(file);
        if (unpaddedSize == SIZE_MAX) return false;
        uncompressedSize = integer.Get(file);
        return uncompressedSize != SIZE_MAX;
    }
    Array<uint8_t> XZRecord::Get(const Sequence<uint8_t>& data_, XZFlags::CheckType checkType, uint8_t& paddingSize) const {
        const Array<XZFilter> filters = MakeArray<XZFilter>(
            XZFilter(XZFilter::Type::LZMA2, MakeArray<uint8_t>(0))
        );
        const Array<uint8_t> data = CollectionToArray<uint8_t>(data_);
        Array<uint8_t> compressed = data;
        for (const XZFilter& filter : filters) compressed = filter.Encrypt(compressed, CipherKey());
        ByteArray ret;
        if (!ret.Write<uint8_t>(0)) return Array<uint8_t>();
        const XZBlockFlags flags = XZBlockFlags(filters.GetSize(), true, true);
        if (!ret.Write<XZBlockFlags>(flags)) return Array<uint8_t>();
        if (flags.hasCompressedSize) {
            const size_t size = compressed.GetSize();
            if (!XZVariableLengthInteger().EncryptWritable(ret, Array<uint8_t>((const uint8_t*)&size, sizeof(size_t)), CipherKey())) return Array<uint8_t>();
        }
        if (flags.hasUncompressedSize) {
            const size_t size = data.GetSize();
            if (!XZVariableLengthInteger().EncryptWritable(ret, Array<uint8_t>((const uint8_t*)&size, sizeof(size_t)), CipherKey())) return Array<uint8_t>();
        }
        for (const XZFilter& filter : filters)
            if (!filter.Save(ret)) return Array<uint8_t>();
        while (ret.GetSize() <= 4 || ret.GetSize() % 4)
            if (!ret.Write<uint8_t>(0)) return Array<uint8_t>();
        ret.AtUnsafe(0) = ret.GetSize() / 4;
        if (!ret.WriteCollection<uint8_t>(CRC().Encrypt(ret, xzCRC32Key))) return Array<uint8_t>();
        if (!ret.WriteCollection<uint8_t>(compressed)) return Array<uint8_t>();
        for (paddingSize = 0; ret.GetSize() % 4; paddingSize++)
            if (!ret.Write<uint8_t>(0)) return Array<uint8_t>();
        switch (checkType) {
            case XZFlags::CheckType::None: break;
            case XZFlags::CheckType::CRC32: {
                if (!ret.WriteCollection<uint8_t>(CRC().Encrypt(data, xzCRC32Key))) return Array<uint8_t>();
                break;
            }
            case XZFlags::CheckType::CRC64: {
                const CipherKey crc64Key = CipherKey(MakeArray<CipherKey>(
                    CipherKey(ByteArray::ToByteArray<size_t>(64)),
                    CipherKey(ByteArray::ToByteArray<CRC::Polynomial>(CRC::Polynomial::CRC64ECMA)),
                    CipherKey(MakeArray<uint8_t>(true)),
                    CipherKey(MakeArray<uint8_t>(true)),
                    CipherKey(ByteArray::ToByteArray<uint64_t>(UINT64_MAX)),
                    CipherKey(MakeArray<uint8_t>(true))
                ));
                if (!ret.WriteCollection<uint8_t>(CRC().Encrypt(data, crc64Key))) return Array<uint8_t>();
                break;
            }
            default: return Array<uint8_t>();
        }
        return ret.GetArray();
    }
    Array<uint8_t> XZRecord::Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const {
        if (key.type != CipherKey::Type::Normal || key.data.GetSize() != sizeof(XZFlags::CheckType)) return Array<uint8_t>();
        uint8_t paddingSize;
        return Get(data, key.data.AsT<XZFlags::CheckType>().Get(), paddingSize);
    }
    Array<uint8_t> XZRecord::DecryptReadablePartial(Readable& readable, const CipherKey& key, const Interval<size_t>& range) const {
        if (key.type != CipherKey::Type::MultiKey || (key.children.GetSize() != 1 && key.children.GetSize() != 2)) return Array<uint8_t>();
        const CipherKey key1 = key.children.AtUnsafe(0);
        if (key1.type != CipherKey::Type::Normal || key1.data.GetSize() != sizeof(XZFlags::CheckType)) return Array<uint8_t>();
        bool noSignatureCheck = false;
        if (key.children.GetSize() == 2) {
            const CipherKey key2 = key.children.AtUnsafe(2);
            if (key2.type != CipherKey::Type::Normal || key2.data.GetSize() != sizeof(bool)) return Array<uint8_t>();
            noSignatureCheck = key2.data.AsT<bool>().Get();
        }
        const XZFlags::CheckType checkType = key1.data.AsT<XZFlags::CheckType>().Get();
        const size_t checkSize = XZFlags::GetSizeOfCheck(checkType);
        uint8_t headerSize;
        if (!readable.Read<uint8_t>(headerSize) || !headerSize) return Array<uint8_t>();
        XZBlockFlags flags;
        if (!readable.Read<XZBlockFlags>(flags)) return Array<uint8_t>();
        ByteArray array = MakeArray<uint8_t>(headerSize, *(const uint8_t*)&flags);
        SavedReadable savedReadable = SavedReadable(readable, array);
        if (!array.Seek(0, SeekMode::End)) return Array<uint8_t>();
        const XZVariableLengthInteger integer;
        const size_t blockCompressedSize = flags.hasCompressedSize ? integer.Get(savedReadable) : 0;
        if (blockCompressedSize == SIZE_MAX) return Array<uint8_t>();
        const size_t blockUncompressedSize = flags.hasUncompressedSize ? integer.Get(savedReadable) : 0;
        if (blockUncompressedSize == SIZE_MAX) return Array<uint8_t>();
        Array<XZFilter> filters = flags.filters + 1;
        for (uint8_t i = 0; i <= flags.filters; i++)
            if (!filters.AtUnsafe(i).Load(savedReadable)) return Array<uint8_t>();
        const uint16_t realHeaderSize = (headerSize + 1) * 4;
        const uint16_t paddingSize = realHeaderSize - array.GetSize() - sizeof(uint32_t);
        for (uint16_t i = 0; i < paddingSize; i++) {
            uint8_t tmp;
            if (!readable.Read<uint8_t>(tmp) || tmp || !array.Add(tmp)) return Array<uint8_t>();
        }
        uint32_t crc32;
        if (!readable.Read<uint32_t>(crc32)) return Array<uint8_t>();
        const Array<uint8_t> tmp = CRC().Encrypt(array, xzCRC32Key);
        const size_t start = array.GetSize() + checkSize + sizeof(uint32_t);
        if (tmp.GetSize() != sizeof(uint32_t) || crc32 != *(const uint32_t*)tmp.GetValue() || !array.Reset()) return Array<uint8_t>();
        for (uint32_t i = start; i < unpaddedSize; i++) {
            uint8_t tmp;
            if (!readable.Read<uint8_t>(tmp) || !array.Add(tmp)) return Array<uint8_t>();
        }
        for (uint8_t i = 0; (array.GetSize() + i) % 4; i++) {
            uint8_t tmp;
            if (!readable.Read<uint8_t>(tmp)) return Array<uint8_t>();
        }
        uint8_t expectedCheck[checkSize];
        if (!readable.ReadBuffer(expectedCheck, checkSize)) return Array<uint8_t>();
        for (uint8_t i = flags.filters + 1; i; i--) array = filters.AtUnsafe(i - 1).Decrypt(array, CipherKey());
        const size_t rangeStart = range.GetMin();
        const size_t rangeEnd = Min<size_t>(range.GetMax(), array.GetSize());
        if (rangeStart >= rangeEnd || array.GetSize() != uncompressedSize) return Array<uint8_t>();
        if (noSignatureCheck) return CollectionToArray<uint8_t>(SubSequence<uint8_t>(array, Interval<size_t>(rangeStart, rangeEnd)));
        Array<uint8_t> check;
        switch (checkType) {
            case XZFlags::CheckType::None: break;
            case XZFlags::CheckType::CRC32: {
                check = CRC().Encrypt(array, xzCRC32Key);
                break;
            }
            case XZFlags::CheckType::CRC64: {
                const CipherKey crc64Key = CipherKey(MakeArray<CipherKey>(
                    CipherKey(ByteArray::ToByteArray<size_t>(64)),
                    CipherKey(ByteArray::ToByteArray<CRC::Polynomial>(CRC::Polynomial::CRC64ECMA)),
                    CipherKey(MakeArray<uint8_t>(true)),
                    CipherKey(MakeArray<uint8_t>(true)),
                    CipherKey(ByteArray::ToByteArray<uint64_t>(UINT64_MAX)),
                    CipherKey(MakeArray<uint8_t>(true))
                ));
                check = CRC().Encrypt(array, crc64Key);
                break;
            }
            default: return Array<uint8_t>();
        }
        return check == ExternArray<uint8_t>(expectedCheck, checkSize) ? CollectionToArray<uint8_t>(SubSequence<uint8_t>(array, Interval<size_t>(rangeStart, rangeEnd))) : Array<uint8_t>();
    }
}