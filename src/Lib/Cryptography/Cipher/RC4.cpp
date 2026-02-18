#include "RC4.hpp"
#include "../../Interfaces/Sequence/FixedSizeCollection.hpp"

namespace MathLib {
    CipherKey RC4::GenerateKey(const CipherKey& key) {
        if (key.type != CipherKey::Type::Normal || key.data.IsEmpty()) return CipherKey();
        const size_t keySize = key.data.GetSize();
        FixedSizeCollection<uint8_t, 256> ret;
        for (uint16_t i = 0; i < ret.GetSize(); i++) ret.AtUnsafe(i) = i;
        uint8_t j = 0;
        for (uint16_t i = 0; i < ret.GetSize(); i++) {
            j += ret.AtUnsafe(i) + key.data.AtUnsafe(i % keySize);
            if (!ret.SwapUnsafe(i, j)) return CipherKey();
        }
        return CipherKey(ret);
    }
    Array<uint8_t> RC4::Convert(const Sequence<uint8_t>& data, const CipherKey& key, size_t limit) const {
        if (key.type != CipherKey::Type::Normal || key.data.GetSize() != 256) return Array<uint8_t>();
        const size_t size = Min<size_t>(data.GetSize(), limit);
        Array<uint8_t> keyData = CollectionToArray<uint8_t>(key.data);
        uint8_t i = 0;
        uint8_t j = 0;
        Array<uint8_t> ret = size;
        for (size_t k = 0; k < size; k++) {
            j += keyData.AtUnsafe(++i);
            if (!keyData.SwapUnsafe(i, j)) return Array<uint8_t>();
            ret.AtUnsafe(k) = data.AtUnsafe(k) ^ keyData.AtUnsafe((uint8_t)(keyData.AtUnsafe(i) + keyData.AtUnsafe(j)));
        }
        return ret;
    }
}