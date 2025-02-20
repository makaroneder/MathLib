#include "FATExtendedBootRecord.hpp"
#include "../../Typedefs.hpp"

namespace MathLib {
    bool FATExtendedBootRecord::IsValid(void) const {
        StartAndReturnFromBenchmark(type == Type::NoID || type == Type::Normal);
    }
}