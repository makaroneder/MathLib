#include "FATExtendedBootRecord32.hpp"
#include "../../Typedefs.hpp"

namespace MathLib {
    bool FATExtendedBootRecord32::IsValid(void) const {
        StartAndReturnFromBenchmark(ebr.IsValid());
    }
}