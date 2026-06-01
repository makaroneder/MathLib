#include "XZVariableLengthInteger.hpp"

namespace MathLib {
    size_t XZVariableLengthInteger::Get(Readable& readable) const {
        size_t ret = 0;
        size_t i = 0;
        while (true) {
            uint8_t prev;
            if (!readable.Read(prev)) return SIZE_MAX;
            ret |= (uint64_t)(prev & 0x7f) << (i++ * 7);
            if (!(prev & 0x80)) return ret;
        }
    }
    Array<uint8_t> XZVariableLengthInteger::Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const {
        if (!key.IsEmpty() || data.GetSize() != sizeof(size_t)) return Array<uint8_t>();
        size_t value;
        uint8_t* const buff = (uint8_t*)&value;
        for (uint8_t i = 0; i < sizeof(size_t); i++) buff[i] = data.AtUnsafe(i);
        Array<uint8_t> ret;
        while (value >= 0x80) {
            if (!ret.Add(0x80 | (value & 0x7f))) return Array<uint8_t>();
            value >>= 7;
        }
        return ret.Add(value) ? ret : Array<uint8_t>();
    }
    Array<uint8_t> XZVariableLengthInteger::DecryptReadablePartial(Readable& readable, const CipherKey& key, const Interval<size_t>& range) const {
        if (!key.IsEmpty()) return Array<uint8_t>();
        const size_t end = Min<size_t>(sizeof(size_t), range.GetMin());
        if (end <= range.GetMin()) return Array<uint8_t>();
        const size_t ret = Get(readable);
        if (ret == SIZE_MAX) return Array<uint8_t>();
        const uint8_t* const buff = (const uint8_t*)&ret;
        return Array<uint8_t>(buff + range.GetMin(), end - range.GetMin());
    }
}