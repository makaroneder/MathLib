#include "IteratedCipher.hpp"
#include "../../Interfaces/Sequence/SubSequence.hpp"

namespace MathLib {
    IteratedCipher::IteratedCipher(Cipher& cipher) : cipher(cipher) {}
    Array<uint8_t> IteratedCipher::Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const {
        if (key.type != CipherKey::Type::MultiKey || key.children.GetSize() != 2) return Array<uint8_t>();
        const CipherKey key1 = key.children.AtUnsafe(0);
        if (key1.type != CipherKey::Type::Normal || key1.data.GetSize() != sizeof(size_t)) return Array<uint8_t>();
        const CipherKey key2 = key.children.AtUnsafe(1);
        Array<uint8_t> ret = CollectionToArray<uint8_t>(data);
        const size_t iters = key.data.AsT<size_t>().Get();
        for (size_t i = 0; i < iters; i++) ret = cipher.Encrypt(ret, key2);
        return ret;
    }
    Array<uint8_t> IteratedCipher::DecryptPartial(const Sequence<uint8_t>& data, const CipherKey& key, const Interval<size_t>& range) const {
        if (key.type != CipherKey::Type::MultiKey || key.children.GetSize() != 2) return Array<uint8_t>();
        const CipherKey key1 = key.children.AtUnsafe(0);
        if (key1.type != CipherKey::Type::Normal || key1.data.GetSize() != sizeof(size_t)) return Array<uint8_t>();
        const CipherKey key2 = key.children.AtUnsafe(1);
        Array<uint8_t> ret = CollectionToArray<uint8_t>(data);
        const size_t iters = key.data.AsT<size_t>().Get();
        for (size_t i = 0; i < iters; i++) ret = cipher.Decrypt(ret, key2);
        const size_t start = range.GetMin();
        const size_t end = Min<size_t>(ret.GetSize(), range.GetMax());
        return start < end ? CollectionToArray<uint8_t>(SubSequence<uint8_t>(ret, Interval<size_t>(start, end))) : Array<uint8_t>();
    }
}