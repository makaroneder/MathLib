#include "BSDChecksum.hpp"

namespace MathLib {
    Array<uint8_t> BSDChecksum::Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key) const {
        StartBenchmark
        if (!key.IsEmpty()) ReturnFromBenchmark(Array<uint8_t>());
        const size_t size = data.GetSize();
        uint16_t ret = 0;
        for (size_t i = 0; i < size; i++)
            ret = (ret >> 1) + ((ret & 1) << 15) + data.At(i);
        ReturnFromBenchmark(Array<uint8_t>((const uint8_t*)&ret, sizeof(uint16_t)));
    }
}