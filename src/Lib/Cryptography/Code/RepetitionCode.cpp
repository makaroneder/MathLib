#include "RepetitionCode.hpp"
#include "../../Bitmap.hpp"

namespace MathLib {
    Array<uint8_t> RepetitionCode::Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key) const {
        StartBenchmark
        if (key.GetSize() != 1) ReturnFromBenchmark(Array<uint8_t>());
        const size_t n = key.At(0);
        const Bitmap tmp = Bitmap(data);
        const size_t size = tmp.GetSize();
        Bitmap ret = Bitmap(size * n);
        for (size_t i = 0; i < size; i++) {
            const Expected<bool> val = tmp.Get(i);
            if (!val.HasValue()) ReturnFromBenchmark(Array<uint8_t>());
            for (size_t j = 0; j < n; j++)
                if (!ret.Set(n * i + j, val.Get())) ReturnFromBenchmark(Array<uint8_t>());
        }
        ReturnFromBenchmark(ret.GetArray());
    }
    Array<uint8_t> RepetitionCode::DecryptPartial(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key, const Interval<size_t>& range) const {
        StartBenchmark
        if (key.GetSize() != 1) ReturnFromBenchmark(Array<uint8_t>());
        const size_t n = key.At(0);
        const Bitmap tmp = Bitmap(data);
        const size_t start = range.GetMin() * 8;
        const size_t end = Min<size_t>(tmp.GetSize() / n, range.GetMax() * 8);
        if (start >= end) ReturnFromBenchmark(Array<uint8_t>());
        Bitmap ret = Bitmap(end - start);
        for (size_t i = start; i < end; i++) {
            size_t ones = 0;
            for (size_t j = 0; j < n; j++) {
                const Expected<bool> val = tmp.Get(n * i + j);
                if (!val.HasValue()) ReturnFromBenchmark(Array<uint8_t>());
                ones += val.Get();
            }
            if (!ret.Set(i - start, ones * 2 >= n)) ReturnFromBenchmark(Array<uint8_t>());
        }
        ReturnFromBenchmark(ret.GetArray());
    }
}