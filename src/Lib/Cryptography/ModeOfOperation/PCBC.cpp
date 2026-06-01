#include "PCBC.hpp"
#include "../../Interfaces/Sequence/SubSequence.hpp"

namespace MathLib {
    PCBC::PCBC(Cipher& cipher) : cipher(cipher) {}
    Array<uint8_t> PCBC::Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const {
        if (key.type != CipherKey::Type::MultiKey || key.children.GetSize() != 2) return Array<uint8_t>();
        const CipherKey key1 = key.children.AtUnsafe(0);
        if (key1.type != CipherKey::Type::Normal) return Array<uint8_t>();
        const CipherKey key2 = key.children.AtUnsafe(1);
        const size_t size = data.GetSize();
        Array<uint8_t> prev = key1.data.GetArray();
        const size_t blockSize = prev.GetSize();
        if (size % blockSize) return Array<uint8_t>();
        Array<uint8_t> ret;
        for (size_t i = 0; i < size; i += blockSize) {
            Array<uint8_t> sub = blockSize;
            for (size_t j = 0; j < blockSize; j++) {
                const uint8_t tmp = data.AtUnsafe(i + j);
                sub.AtUnsafe(j) = tmp;
                prev.AtUnsafe(j) ^= tmp;
            }
            prev = cipher.Encrypt(prev, key2);
            if (prev.GetSize() != blockSize || !ret.AddSequence(prev)) return Array<uint8_t>();
            for (size_t j = 0; j < blockSize; j++) prev.AtUnsafe(j) ^= sub.AtUnsafe(j);
        }
        return ret;
    }
    Array<uint8_t> PCBC::DecryptPartial(const Sequence<uint8_t>& data, const CipherKey& key, const Interval<size_t>& range) const {
        if (key.type != CipherKey::Type::MultiKey || key.children.GetSize() != 2) return Array<uint8_t>();
        const size_t min = range.GetMin();
        const size_t max = range.GetMax();
        if (min >= max) return Array<uint8_t>();
        const CipherKey key1 = key.children.AtUnsafe(0);
        if (key1.type != CipherKey::Type::Normal) return Array<uint8_t>();
        const CipherKey key2 = key.children.AtUnsafe(1);
        const size_t size = data.GetSize();
        Array<uint8_t> prev = key1.data.GetArray();
        const size_t blockSize = prev.GetSize();
        if (size % blockSize) return Array<uint8_t>();
        Array<uint8_t> ret;
        for (size_t i = 0; i < size; i += blockSize) {
            Array<uint8_t> sub = blockSize;
            for (size_t j = 0; j < blockSize; j++) sub.AtUnsafe(j) = data.AtUnsafe(i + j);
            const Array<uint8_t> tmp = cipher.Decrypt(prev, key2);
            if (tmp.GetSize() != blockSize) return Array<uint8_t>();
            for (size_t j = 0; j < blockSize; j++) prev.AtUnsafe(j) ^= tmp.AtUnsafe(j);
            if (!ret.AddSequence(prev)) return Array<uint8_t>();
            for (size_t j = 0; j < blockSize; j++) prev.AtUnsafe(j) ^= sub.AtUnsafe(j);
        }
        const size_t end = Min<size_t>(max, size);
        return min < end ? CollectionToArray<uint8_t>(SubSequence<uint8_t>(ret, Interval<size_t>(min, end))) : Array<uint8_t>();
    }
}