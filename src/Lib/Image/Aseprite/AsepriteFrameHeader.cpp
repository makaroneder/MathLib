#include "AsepriteFrameHeader.hpp"
#include "../../Typedefs.hpp"

namespace MathLib {
    bool AsepriteFrameHeader::IsValid(void) const {
        StartBenchmark
        ReturnFromBenchmark(signature == expectedSignature);
    }
}