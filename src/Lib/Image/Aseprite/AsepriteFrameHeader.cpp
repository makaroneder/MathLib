#include "AsepriteFrameHeader.hpp"
#include "../../Typedefs.hpp"

namespace MathLib {
    bool AsepriteFrameHeader::IsValid(void) const {
        StartAndReturnFromBenchmark(signature == expectedSignature);
    }
}