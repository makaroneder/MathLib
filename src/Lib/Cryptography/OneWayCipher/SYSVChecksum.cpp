#include "SYSVChecksum.hpp"

namespace MathLib {
    Array<uint8_t> SYSVChecksum::Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key) const {
        StartBenchmark
        if (!key.IsEmpty()) ReturnFromBenchmark(Array<uint8_t>());
        const size_t size = data.GetSize();
        uint32_t sum = 0;
        for (size_t i = 0; i < size; i++) sum += data.At(i);
        const uint32_t tmp = (uint16_t)sum + sum / (UINT16_MAX + 1);
        const uint16_t ret = (uint16_t)tmp + tmp / (UINT16_MAX + 1);
        ReturnFromBenchmark(Array<uint8_t>((const uint8_t*)&ret, sizeof(uint16_t)));
    }
}