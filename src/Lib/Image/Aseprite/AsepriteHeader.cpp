#include "AsepriteHeader.hpp"
#include "../../Typedefs.hpp"

namespace MathLib {
    bool AsepriteHeader::IsValid(void) const {
        StartAndReturnFromBenchmark(signature == expectedSignature);
    }
}