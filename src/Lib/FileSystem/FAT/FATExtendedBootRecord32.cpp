#include "FATExtendedBootRecord32.hpp"
#include "../../Benchmark.hpp"

namespace MathLib {
    bool FATExtendedBootRecord32::IsValid(void) const {
        StartAndReturnFromBenchmark(ebr.IsValid());
    }
}