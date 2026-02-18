#include "AffineCipher.hpp"
#include "../../Host.hpp"

namespace MathLib {
    Array<uint8_t> AffineCipher::Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const {
        if (key.type != CipherKey::Type::MultiKey || key.children.GetSize() != 2) return Array<uint8_t>();
        const CipherKey key1 = key.children.AtUnsafe(0);
        if (key1.type != CipherKey::Type::Normal || key1.data.GetSize() != sizeof(ssize_t)) return Array<uint8_t>();
        const CipherKey key2 = key.children.AtUnsafe(1);
        if (key2.type != CipherKey::Type::Normal || key2.data.GetSize() != sizeof(ssize_t)) return Array<uint8_t>();
        const ssize_t a = key1.data.AsT<ssize_t>().Get();
        const ssize_t b = key2.data.AsT<ssize_t>().Get();
        Array<uint8_t> ret = Array<uint8_t>(data.GetSize());
        for (size_t i = 0; i < ret.GetSize(); i++) ret.AtUnsafe(i) = data.AtUnsafe(i) * a + b;
        return ret;
    }
    Array<uint8_t> AffineCipher::DecryptPartial(const Sequence<uint8_t>& data, const CipherKey& key, const Interval<size_t>& range) const {
        const size_t start = range.GetMin();
        const size_t end = Min<size_t>(data.GetSize(), range.GetMax());
        if (start >= end || key.type != CipherKey::Type::MultiKey || key.children.GetSize() != 2) return Array<uint8_t>();
        const CipherKey key1 = key.children.AtUnsafe(0);
        if (key1.type != CipherKey::Type::Normal || key1.data.GetSize() != sizeof(ssize_t)) return Array<uint8_t>();
        const CipherKey key2 = key.children.AtUnsafe(1);
        if (key2.type != CipherKey::Type::Normal || key2.data.GetSize() != sizeof(ssize_t)) return Array<uint8_t>();
        const ssize_t a = key1.data.AsT<ssize_t>().Get();
        const ssize_t b = key2.data.AsT<ssize_t>().Get();
        Array<uint8_t> ret = Array<uint8_t>(end - start);
        for (size_t i = start; i < end; i++) ret.AtUnsafe(i - start) = (data.AtUnsafe(i) - b) / a;
        return ret;
    }
}