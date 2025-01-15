#include "AsepriteHeader.hpp"
#include "../../Typedefs.hpp"

namespace MathLib {
    bool AsepriteHeader::IsValid(void) const {
        StartBenchmark
        ReturnFromBenchmark(signature == expectedSignature);
    }
}