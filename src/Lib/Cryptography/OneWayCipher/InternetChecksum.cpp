#include "InternetChecksum.hpp"

namespace MathLib {
    uint16_t InternetChecksum::Get(const Sequence<uint8_t>& data) const {
        const size_t size = data.GetSize();
        uint8_t buff[2];
        uint32_t ret = 0;
        for (size_t i = 0; i < size; i += 2)  {
            buff[0] = data.At(i);
            buff[1] = data.At(i + 1);
            ret += *(const uint16_t*)buff;
        }
        if (size % 2) ret += data.At(size - 1);
        while (ret >> 16) ret = (uint16_t)ret + (ret >> 16);
        return ~ret;
    }
    Array<uint8_t> InternetChecksum::Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key) const {
        StartBenchmark
        if (!key.IsEmpty()) ReturnFromBenchmark(MathLib::Array<uint8_t>());
        const uint16_t ret = Get(data);
        ReturnFromBenchmark(Array<uint8_t>((const uint8_t*)&ret, sizeof(uint16_t)));
    }
}