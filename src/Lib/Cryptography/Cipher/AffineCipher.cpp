#include "AffineCipher.hpp"
#include "../../Host.hpp"

namespace MathLib {
    Array<uint8_t> AffineCipher::Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key) const {
        StartBenchmark
        if (key.GetSize() != 2) ReturnFromBenchmark(Array<uint8_t>());
        const ssize_t a = key.At(0);
        const ssize_t b = key.At(1);
        Array<uint8_t> ret = Array<uint8_t>(data.GetSize());
        for (size_t i = 0; i < ret.GetSize(); i++) ret.At(i) = data.At(i) * a + b;
        ReturnFromBenchmark(ret);
    }
    Array<uint8_t> AffineCipher::DecryptPartial(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key, const Interval<size_t>& range) const {
        StartBenchmark
        const size_t start = range.GetMin();
        const size_t end = Min<size_t>(data.GetSize(), range.GetMax());
        if (start >= end || key.GetSize() != 2) ReturnFromBenchmark(Array<uint8_t>());
        const ssize_t a = key.At(0);
        const ssize_t b = key.At(1);
        Array<uint8_t> ret = Array<uint8_t>(end - start);
        for (size_t i = start; i < end; i++) ret.At(i - start) = (data.At(i) - b) / a;
        ReturnFromBenchmark(ret);
    }
}