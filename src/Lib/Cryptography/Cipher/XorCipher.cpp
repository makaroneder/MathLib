#include "XorCipher.hpp"
#include "../../Interfaces/Sequence/WrapSequence.hpp"

namespace MathLib {
    Array<uint8_t> XorCipher::Convert(const Sequence<uint8_t>& data, const CipherKey& key, size_t limit) const {
        if (key.type != CipherKey::Type::Normal || key.data.IsEmpty()) return Array<uint8_t>();
        const size_t size = Min<size_t>(data.GetSize(), limit);
        const WrapSequence<uint8_t> wrap = WrapSequence<uint8_t>(key.data, size);
        Array<uint8_t> ret = size;
        for (size_t i = 0; i < size; i++) ret.AtUnsafe(i) = data.AtUnsafe(i) ^ wrap.AtUnsafe(i);
        return ret;
    }
}