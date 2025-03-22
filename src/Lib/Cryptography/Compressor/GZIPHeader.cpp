#include "GZIPHeader.hpp"
#include "../../Typedefs.hpp"

namespace MathLib {
    bool GZIPHeader::IsValid(void) const {
        StartAndReturnFromBenchmark(signature == expectedSignature);
    }
}