#include "NullCipher.hpp"
#include "../../Interfaces/Sequence/RandomSequence.hpp"

namespace MathLib {
    Array<uint8_t> NullCipher::Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const {
        if (key.type != CipherKey::Type::MultiKey || key.children.GetSize() != 2) return Array<uint8_t>();
        const CipherKey key1 = key.children.AtUnsafe(0);
        if (key1.type != CipherKey::Type::Normal || key1.data.GetSize() != sizeof(size_t)) return Array<uint8_t>();
        const CipherKey key2 = key.children.AtUnsafe(1);
        if (key2.type != CipherKey::Type::Normal || key2.data.GetSize() != sizeof(size_t)) return Array<uint8_t>();
        const size_t blockSize = key1.data.AsT<size_t>().Get();
        const size_t blockPosition = key2.data.AsT<size_t>().Get();
        const size_t size = data.GetSize();
        Array<uint8_t> ret = CollectionToArray<uint8_t>(RandomSequence<uint8_t>(Interval<uint8_t>(0, UINT8_MAX), blockSize * size));
        for (size_t i = 0; i < size; i++) ret.At(i * blockSize + blockPosition) = data.At(i);
        return ret;
    }
    Array<uint8_t> NullCipher::DecryptPartial(const Sequence<uint8_t>& data, const CipherKey& key, const Interval<size_t>& range) const {
        if (key.type != CipherKey::Type::MultiKey || key.children.GetSize() != 2) return Array<uint8_t>();
        const CipherKey key1 = key.children.AtUnsafe(0);
        if (key1.type != CipherKey::Type::Normal || key1.data.GetSize() != sizeof(size_t)) return Array<uint8_t>();
        const CipherKey key2 = key.children.AtUnsafe(1);
        if (key2.type != CipherKey::Type::Normal || key2.data.GetSize() != sizeof(size_t)) return Array<uint8_t>();
        const size_t blockSize = key1.data.AsT<size_t>().Get();
        const size_t blockPosition = key2.data.AsT<size_t>().Get();
        const size_t size = data.GetSize();
        if (size % blockSize) return Array<uint8_t>();
        const size_t blocks = size / blockSize;
        const size_t start = range.GetMin();
        const size_t end = Min<size_t>(blocks, range.GetMax());
        if (start >= end) return Array<uint8_t>();
        Array<uint8_t> ret = Array<uint8_t>(end - start);
        for (size_t i = start; i < end; i++) ret.At(i - start) = data.At(i * blockSize + blockPosition);
        return ret;
    }
}