#include "FATLongFileName.hpp"
#include "../../Typedefs.hpp"

namespace MathLib {
    bool FATLongFileName::IsValid(void) const {
        StartAndReturnFromBenchmark(signature == expectedSignature);
    }
}