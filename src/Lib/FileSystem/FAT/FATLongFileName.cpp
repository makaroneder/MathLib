#include "FATLongFileName.hpp"
#include "../../Benchmark.hpp"

namespace MathLib {
    bool FATLongFileName::IsValid(void) const {
        StartAndReturnFromBenchmark(signature == expectedSignature);
    }
}