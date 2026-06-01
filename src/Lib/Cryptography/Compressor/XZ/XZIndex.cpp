#include "XZIndex.hpp"
#include "XZCRC32Key.hpp"
#include "XZVariableLengthInteger.hpp"
#include "../../../Interfaces/SavedReadable.hpp"

namespace MathLib {
    XZIndex::XZIndex(void) : records() {}
    XZIndex::XZIndex(const Array<XZRecord>& records) : records(records) {}
    bool XZIndex::Save(Writable& file) const {
        const XZVariableLengthInteger integer;
        ByteArray ret;
        if (!ret.Write<uint8_t>(0)) return false;
        const size_t size = records.GetSize();
        if (!integer.EncryptWritable(ret, Array<uint8_t>((const uint8_t*)&size, sizeof(size_t)), CipherKey())) return false;
        for (const XZRecord& record : records)
            if (!record.Save(ret)) return false;
        while (ret.GetSize() % 4)
            if (!ret.Write<uint8_t>(0)) return false;
        return file.WriteCollection<uint8_t>(ret) && file.WriteCollection<uint8_t>(CRC().Encrypt(ret, xzCRC32Key));
    }
    bool XZIndex::Load(Readable& file) {
        ByteArray arr;
        SavedReadable savedReadable = SavedReadable(file, arr);
        uint8_t zero;
        if (!savedReadable.Read<uint8_t>(zero) || zero) return false;
        const XZVariableLengthInteger integer;
        records = integer.Get(savedReadable);
        for (XZRecord& record : records)
            if (!record.Load(savedReadable)) return false;
        while (arr.GetSize() % 4) {
            uint8_t zero;
            if (!savedReadable.Read<uint8_t>(zero) || zero) return false;
        }
        uint32_t crc32;
        if (!file.Read<uint32_t>(crc32)) return false;
        const Array<uint8_t> crc = CRC().Encrypt(arr, xzCRC32Key);
        return crc.GetSize() == sizeof(uint32_t) && crc32 == *(const uint32_t*)crc.GetValue();
    }
}