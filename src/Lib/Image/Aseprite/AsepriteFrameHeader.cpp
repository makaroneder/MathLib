#include "AsepriteFrameHeader.hpp"
#include "../../Benchmark.hpp"

namespace MathLib {
    AsepriteFrameHeader::AsepriteFrameHeader(void) : size(0), signature(expectedSignature), chunks16(0), duration(0), reserved(0), chunks32(0) {
        EmptyBenchmark
    }
    bool AsepriteFrameHeader::IsValid(void) const {
        StartAndReturnFromBenchmark(signature == expectedSignature);
    }
}