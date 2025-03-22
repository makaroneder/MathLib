#include "RunLengthEncoding.hpp"

namespace MathLib {
    Array<uint8_t> RunLengthEncoding::Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key, bool encrypt) const {
        StartBenchmark
        const size_t size = data.GetSize();
        if (key.GetSize() != 1 || !size) ReturnFromBenchmark(Array<uint8_t>());
        const bool sizeFirst = key.At(0);
        Array<uint8_t> ret;
        if (encrypt) {
            uint8_t prev = data.At(0);
            uint8_t count = 0;
            for (size_t i = 1; i < size; i++) {
                const uint8_t tmp = data.At(i);
                if (prev != tmp || count == UINT8_MAX) {
                    if (sizeFirst) Swap<uint8_t>(count, prev);
                    if (!ret.Add(prev) || !ret.Add(count)) ReturnFromBenchmark(Array<uint8_t>());
                    count = 0;
                    prev = tmp;
                }
                else count++;
            }
            if (sizeFirst) Swap<uint8_t>(count, prev);
            ReturnFromBenchmark(ret.Add(prev) && ret.Add(count) ? ret : Array<uint8_t>());
        }
        for (size_t i = 0; i < size; i += 2) {
            const uint8_t count = data.At(i + !sizeFirst);
            const uint8_t x = data.At(i + sizeFirst);
            for (uint16_t j = 0; j <= count; j++)
            if (!ret.Add(x)) ReturnFromBenchmark(Array<uint8_t>());
        }
        ReturnFromBenchmark(ret);
    }
}