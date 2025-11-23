#include "NullCipher.hpp"
#include "../../Interfaces/Sequence/RandomSequence.hpp"

namespace MathLib {
    Array<uint8_t> NullCipher::Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key) const {
        if (key.GetSize() != 2) return Array<uint8_t>();
        const size_t blockSize = key.At(0);
        const size_t blockPosition = key.At(1);
        const size_t size = data.GetSize();
        Array<uint8_t> ret = CollectionToArray<uint8_t>(RandomSequence<uint8_t>(Interval<uint8_t>(0, UINT8_MAX), blockSize * size));
        for (size_t i = 0; i < size; i++) ret.At(i * blockSize + blockPosition) = data.At(i);
        return ret;
    }
    Array<uint8_t> NullCipher::DecryptPartial(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key, const Interval<size_t>& range) const {
        if (key.GetSize() != 2) return Array<uint8_t>();
        const size_t blockSize = key.At(0);
        const size_t blockPosition = key.At(1);
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