#include "PolyalphabeticCipher.hpp"
#include "../../Interfaces/WrapSequence.hpp"

namespace MathLib {
    Array<uint8_t> PolyalphabeticCipher::Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key) const {
        StartBenchmark
        const size_t size = data.GetSize();
        const MathLib::WrapSequence<uint64_t> wrap = MathLib::WrapSequence<uint64_t>(key, size);
        MathLib::Array<uint8_t> ret = MathLib::Array<uint8_t>(size);
        for (size_t i = 0; i < size; i++) ret.At(i) = data.At(i) + wrap.At(i);
        ReturnFromBenchmark(ret);
    }
    Array<uint8_t> PolyalphabeticCipher::DecryptPartial(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key, const Interval<size_t>& range) const {
        StartBenchmark
        const size_t size = data.GetSize();
        const size_t start = range.GetMin();
        const size_t end = Min<size_t>(size, range.GetMax());
        if (start >= end) ReturnFromBenchmark(Array<uint8_t>());
        const MathLib::WrapSequence<uint64_t> wrap = MathLib::WrapSequence<uint64_t>(key, size);
        MathLib::Array<uint8_t> ret = MathLib::Array<uint8_t>(end - start);
        for (size_t i = start; i < end; i++) ret.At(i - start) = data.At(i) - wrap.At(i);
        ReturnFromBenchmark(ret);
    }
}