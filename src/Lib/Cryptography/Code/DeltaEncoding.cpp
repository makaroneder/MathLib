#include "DeltaEncoding.hpp"

namespace MathLib {
    Array<uint8_t> DeltaEncoding::Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key) const {
        StartBenchmark
        if (!key.IsEmpty()) ReturnFromBenchmark(MathLib::Array<uint8_t>());
        const size_t size = data.GetSize();
        uint8_t prev = 0;
        MathLib::Array<uint8_t> ret = MathLib::Array<uint8_t>(size);
        for (size_t i = 0; i < size; i++) {
            const uint8_t tmp = data.At(i);
            ret.At(i) = tmp - prev;
            prev = tmp;
        }
        ReturnFromBenchmark(ret);
    }
    Array<uint8_t> DeltaEncoding::DecryptPartial(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key, const Interval<size_t>& range) const {
        StartBenchmark
        if (!key.IsEmpty()) ReturnFromBenchmark(MathLib::Array<uint8_t>());
        const size_t size = data.GetSize();
        const size_t start = range.GetMin();
        const size_t end = MathLib::Min<size_t>(range.GetMax(), size);
        uint8_t prev = 0;
        MathLib::Array<uint8_t> ret = MathLib::Array<uint8_t>(end - start);
        for (size_t i = 0; i < end; i++) {
            prev += data.At(i);
            if (i >= start) ret.At(i - start) = prev;
        }
        ReturnFromBenchmark(ret);
    }
}