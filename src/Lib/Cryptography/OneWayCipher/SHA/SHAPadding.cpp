#include "SHAPadding.hpp"

namespace MathLib {
    Array<uint8_t> SHAPadding::Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const {
        if (!key.IsEmpty()) return Array<uint8_t>();
        Array<uint8_t> ret = CollectionToArray<uint8_t>(data);
        const uint64_t originalSize = ret.GetSize() * 8;
        if (!ret.Add(0x80)) return Array<uint8_t>();
        const uint8_t paddingLength = (56 - (ret.GetSize() % 64) + 64) % 64;
        for (uint8_t i = 0; i < paddingLength; i++)
            if (!ret.Add(0x00)) return Array<uint8_t>();
        for (uint8_t i = 0; i < sizeof(uint64_t); i++)
            if (!ret.Add(originalSize >> (56 - 8 * i))) return Array<uint8_t>();
        return ret;
    }
    Array<uint8_t> SHAPadding::DecryptPartial(const Sequence<uint8_t>& data, const CipherKey& key, const Interval<size_t>& range) const {
        if (!key.IsEmpty()) return Array<uint8_t>();
        const size_t fullSize = data.GetSize();
        if (fullSize % 64) return Array<uint8_t>();
        uint64_t size = 0;
        for (uint8_t i = 0; i < sizeof(uint64_t); i++)
            size |= data.AtUnsafe(fullSize - sizeof(uint64_t) + i) << (56 - 8 * i);
        if (size % 8) return Array<uint8_t>();
        size /= 8;
        if (fullSize <= size + sizeof(uint64_t) || data.AtUnsafe(size) != 0x80) return Array<uint8_t>();
        const size_t start = range.GetMin();
        const size_t end = Min<size_t>(size, range.GetMax());
        if (start >= end) return Array<uint8_t>();
        const uint8_t paddingLength = fullSize - size - sizeof(uint64_t) - 1;
        if (paddingLength >= 64) return Array<uint8_t>();
        for (uint8_t i = 0; i < paddingLength; i++)
            if (data.AtUnsafe(size + i + 1)) return Array<uint8_t>();
        Array<uint8_t> ret = Array<uint8_t>(end - start);
        for (size_t i = start; i < end; i++) ret.AtUnsafe(i - start) = data.AtUnsafe(i);
        return ret;
    }
}