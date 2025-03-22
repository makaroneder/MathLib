#include "RepetitionCode.hpp"
#include "../../Bitmap.hpp"

namespace MathLib {
    Array<uint8_t> RepetitionCode::Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key, bool encrypt) const {
        StartBenchmark
        if (key.GetSize() != 1) ReturnFromBenchmark(Array<uint8_t>());
        const size_t n = key.At(0);
        const Bitmap tmp = Bitmap(data);
        if (encrypt) {
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
        Bitmap ret = Bitmap(tmp.GetSize() / n);
        const size_t size = ret.GetSize();
        for (size_t i = 0; i < size; i++) {
            size_t ones = 0;
            for (size_t j = 0; j < n; j++) {
                const Expected<bool> val = tmp.Get(n * i + j);
                if (!val.HasValue()) ReturnFromBenchmark(Array<uint8_t>());
                ones += val.Get();
            }
            if (!ret.Set(i, ones * 2 >= n)) ReturnFromBenchmark(Array<uint8_t>());
        }
        ReturnFromBenchmark(ret.GetArray());
    }
}