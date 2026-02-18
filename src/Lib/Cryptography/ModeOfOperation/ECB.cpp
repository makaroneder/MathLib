#include "ECB.hpp"
#include "../../Interfaces/Sequence/SubSequence.hpp"

namespace MathLib {
    ECB::ECB(Cipher& cipher) : cipher(cipher) {}
    Array<uint8_t> ECB::Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const {
        if (key.type != CipherKey::Type::MultiKey || key.children.GetSize() != 2) return Array<uint8_t>();
        const CipherKey key1 = key.children.AtUnsafe(0);
        if (key1.type != CipherKey::Type::Normal || key1.data.GetSize() != sizeof(size_t)) return Array<uint8_t>();
        const size_t blockSize = key1.data.AsT<size_t>().Get();
        const CipherKey key2 = key.children.AtUnsafe(1);
        const size_t size = data.GetSize();
        if (size % blockSize) return Array<uint8_t>();
        Array<uint8_t> ret;
        for (size_t i = 0; i < size; i += blockSize)
            if (!ret.AddSequence(cipher.Encrypt(MathLib::SubSequence<uint8_t>(data, MathLib::Interval<size_t>(i, i + blockSize)), key2))) return Array<uint8_t>();
        return ret;
    }
    Array<uint8_t> ECB::DecryptPartial(const Sequence<uint8_t>& data, const CipherKey& key, const Interval<size_t>& range) const {
        if (key.type != CipherKey::Type::MultiKey || key.children.GetSize() != 2) return Array<uint8_t>();
        const size_t min = range.GetMin();
        const size_t max = range.GetMax();
        if (min >= max) return Array<uint8_t>();
        const CipherKey key1 = key.children.AtUnsafe(0);
        if (key1.type != CipherKey::Type::Normal || key1.data.GetSize() != sizeof(size_t)) return Array<uint8_t>();
        const size_t blockSize = key1.data.AsT<size_t>().Get();
        const CipherKey key2 = key.children.AtUnsafe(1);
        const size_t size = data.GetSize();
        if (size % blockSize) return Array<uint8_t>();
        Array<uint8_t> ret;
        for (size_t i = 0; i < size; i += blockSize)
            if (!ret.AddSequence(cipher.Decrypt(MathLib::SubSequence<uint8_t>(data, MathLib::Interval<size_t>(i, i + blockSize)), key2))) return Array<uint8_t>();
        return min < size ? CollectionToArray<uint8_t>(SubSequence<uint8_t>(ret, Interval<size_t>(min, size))) : Array<uint8_t>();
    }
}