#include "NullCipher.hpp"
#include "../../Interfaces/RandomSequence.hpp"

namespace MathLib {
    Array<uint8_t> NullCipher::Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key) const {
        StartBenchmark
        if (key.GetSize() != 2) ReturnFromBenchmark(Array<uint8_t>());
        const size_t blockSize = key.At(0);
        const size_t blockPosition = key.At(1);
        const size_t size = data.GetSize();
        MathLib::Array<uint8_t> ret = MathLib::CollectionToArray<uint8_t>(MathLib::RandomSequence<uint8_t>(MathLib::Interval<uint8_t>(0, UINT8_MAX), blockSize * size));
        for (size_t i = 0; i < size; i++) ret.At(i * blockSize + blockPosition) = data.At(i);
        ReturnFromBenchmark(ret);
    }
    Array<uint8_t> NullCipher::DecryptPartial(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key, const Interval<size_t>& range) const {
        StartBenchmark
        if (key.GetSize() != 2) ReturnFromBenchmark(MathLib::Array<uint8_t>());
        const size_t blockSize = key.At(0);
        const size_t blockPosition = key.At(1);
        const size_t size = data.GetSize();
        if (size % blockSize) ReturnFromBenchmark(MathLib::Array<uint8_t>());
        const size_t blocks = size / blockSize;
        const size_t start = range.GetMin();
        const size_t end = Min<size_t>(blocks, range.GetMax());
        if (start >= end) ReturnFromBenchmark(MathLib::Array<uint8_t>());
        MathLib::Array<uint8_t> ret = MathLib::Array<uint8_t>(end - start);
        for (size_t i = start; i < end; i++) ret.At(i - start) = data.At(i * blockSize + blockPosition);
        ReturnFromBenchmark(ret);
    }
}