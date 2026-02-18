#include "PolyalphabeticCipher.hpp"
#include "../../Interfaces/Sequence/WrapSequence.hpp"

namespace MathLib {
    Array<uint8_t> PolyalphabeticCipher::Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const {
        if (key.type != CipherKey::Type::Normal || key.data.IsEmpty()) return Array<uint8_t>();
        const size_t size = data.GetSize();
        const WrapSequence<uint8_t> wrap = WrapSequence<uint8_t>(key.data, size);
        Array<uint8_t> ret = Array<uint8_t>(size);
        for (size_t i = 0; i < size; i++) ret.AtUnsafe(i) = data.AtUnsafe(i) + wrap.AtUnsafe(i);
        return ret;
    }
    Array<uint8_t> PolyalphabeticCipher::DecryptPartial(const Sequence<uint8_t>& data, const CipherKey& key, const Interval<size_t>& range) const {
        if (key.type != CipherKey::Type::Normal || key.data.IsEmpty()) return Array<uint8_t>();
        const size_t size = data.GetSize();
        const size_t start = range.GetMin();
        const size_t end = Min<size_t>(size, range.GetMax());
        if (start >= end) return Array<uint8_t>();
        const WrapSequence<uint8_t> wrap = WrapSequence<uint8_t>(key.data, size);
        Array<uint8_t> ret = Array<uint8_t>(end - start);
        for (size_t i = start; i < end; i++) ret.AtUnsafe(i - start) = data.AtUnsafe(i) - wrap.AtUnsafe(i);
        return ret;
    }
}