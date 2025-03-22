#include "NullCipher.hpp"
#include "../../Interfaces/RandomSequence.hpp"

namespace MathLib {
    Array<uint8_t> NullCipher::Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key, bool encrypt) const {
        StartBenchmark
        if (key.GetSize() != 2) ReturnFromBenchmark(MathLib::Array<uint8_t>());
        const size_t blockSize = key.At(0);
        const size_t blockPosition = key.At(1);
        const size_t size = data.GetSize();
        if (encrypt) {
            MathLib::Array<uint8_t> ret = MathLib::CollectionToArray<uint8_t>(MathLib::RandomSequence<uint8_t>(MathLib::Interval<uint8_t>(0, UINT8_MAX), blockSize * size));
            for (size_t i = 0; i < size; i++) ret.At(i * blockSize + blockPosition) = data.At(i);
            ReturnFromBenchmark(ret);
        }
        if (size % blockSize) ReturnFromBenchmark(MathLib::Array<uint8_t>());
        const size_t blocks = size / blockSize;
        MathLib::Array<uint8_t> ret = MathLib::Array<uint8_t>(blocks);
        for (size_t i = 0; i < blocks; i++) ret.At(i) = data.At(i * blockSize + blockPosition);
        ReturnFromBenchmark(ret);
    }
}