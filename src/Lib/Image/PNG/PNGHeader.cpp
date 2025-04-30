#include "PNGHeader.hpp"
#include "../../Typedefs.hpp"

namespace MathLib {
    PNGHeader::PNGHeader(void) {
        StartBenchmark
        for (uint8_t i = 0; i < SizeOfArray(signature); i++) signature[i] = expectedSignature[i];
        EndBenchmark
    }
    bool PNGHeader::IsValid(void) const {
        StartAndReturnFromBenchmark(*(const uint64_t*)signature == *(const uint64_t*)expectedSignature);
    }
}