#include "FATLongFileName.hpp"
#include "../../Typedefs.hpp"

namespace MathLib {
    bool FATLongFileName::IsValid(void) const {
        StartBenchmark
        ReturnFromBenchmark(signature == expectedSignature);
    }
}