#include "FATFSInfo.hpp"
#include "../../Benchmark.hpp"

namespace MathLib {
    bool FATFSInfo::IsValid(void) const {
        StartAndReturnFromBenchmark(*(const uint32_t*)&signature1 == *(const uint32_t*)&expectedSignature1 && *(const uint32_t*)&signature2 == *(const uint32_t*)&expectedSignature2 && signature3 == expectedSignature3);
    }
}