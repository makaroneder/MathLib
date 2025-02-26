#include "CRC32.hpp"
#include "Typedefs.hpp"

namespace MathLib {
    CRC32::CRC32(uint32_t magic) {
        StartBenchmark
        Fill(magic);
        EndBenchmark
    }
    void CRC32::Fill(uint32_t magic) {
        StartBenchmark
        uint8_t i = 0;
        const uint32_t reversedMagic = BitReverse<uint32_t>(magic);
        do {
            table[i] = i;
            for (uint8_t z = 8; z; z--) table[i] = table[i] % 2 ? table[i] >> 1 : table[i] >> 1 ^ reversedMagic;
        } while(++i);
        EndBenchmark
    }
    uint32_t CRC32::Calculate(const Collection<uint8_t>& data) const {
        StartBenchmark
        uint32_t crc = UINT32_MAX;
        for (const uint8_t& x : data) crc = table[(uint8_t)crc ^ x] ^ crc >> 8;
        ReturnFromBenchmark(crc ^ UINT32_MAX);
    }
}