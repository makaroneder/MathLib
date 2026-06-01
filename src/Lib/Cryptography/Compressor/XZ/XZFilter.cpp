#include "XZFilter.hpp"
#include "../LZMA2.hpp"
#include "XZVariableLengthInteger.hpp"

namespace MathLib {
    XZFilter::XZFilter(void) : key(), id() {}
    XZFilter::XZFilter(Type id, const Array<uint8_t>& key) : key(key), id(id) {}
    bool XZFilter::Equals(const XZFilter& other) const {
        return id == other.id && key == other.key;
    }
    Array<uint8_t> XZFilter::Encrypt(const Sequence<uint8_t>& data, const CipherKey& key_) const {
        if (!key_.IsEmpty()) return Array<uint8_t>();
        switch (id) {
            case Type::LZMA2: {
                if (key.GetSize() != 1) return Array<uint8_t>();
                return LZMA2().Encrypt(data, CipherKey(MakeArray<uint8_t>(key.AtUnsafe(0) & 0x3f)));
            }
            default: return Array<uint8_t>();
        }
    }
    Array<uint8_t> XZFilter::DecryptPartial(const Sequence<uint8_t>& data, const CipherKey& key_, const Interval<size_t>& range) const {
        if (!key_.IsEmpty()) return Array<uint8_t>();
        switch (id) {
            case Type::LZMA2: {
                if (key.GetSize() != 1) return Array<uint8_t>();
                return LZMA2().DecryptPartial(data, CipherKey(MakeArray<uint8_t>(key.AtUnsafe(0) & 0x3f)), range);
            }
            default: return Array<uint8_t>();
        }
    }
    bool XZFilter::Save(Writable& file) const {
        const size_t size = key.GetSize();
        const XZVariableLengthInteger integer;
        return integer.EncryptWritable(file, Array<uint8_t>((const uint8_t*)&id, sizeof(Type)), CipherKey()) && integer.EncryptWritable(file, Array<uint8_t>((const uint8_t*)&size, sizeof(size_t)), CipherKey()) && file.WriteCollection<uint8_t>(key);
    }
    bool XZFilter::Load(Readable& file) {
        const XZVariableLengthInteger integer;
        id = (XZFilter::Type)integer.Get(file);
        if (id == (XZFilter::Type)SIZE_MAX) return false;
        const size_t size = integer.Get(file);
        if (size == SIZE_MAX) return false;
        key = size;
        for (uint8_t& x : key)
            if (!file.Read<uint8_t>(x)) return false;
        return true;
    }
}