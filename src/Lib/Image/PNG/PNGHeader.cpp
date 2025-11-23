#include "PNGHeader.hpp"
#include "../../Typedefs.hpp"

namespace MathLib {
    PNGHeader::PNGHeader(void) {
        for (uint8_t i = 0; i < SizeOfArray(signature); i++) signature[i] = expectedSignature[i];
    }
    bool PNGHeader::IsValid(void) const {
        return *(const uint64_t*)signature == *(const uint64_t*)expectedSignature;
    }
}