#include "FATExtendedBootRecord.hpp"
#include "../../Typedefs.hpp"

namespace MathLib {
    bool FATExtendedBootRecord::IsValid(void) const {
        StartBenchmark
        ReturnFromBenchmark(type == Type::NoID || type == Type::Normal);
    }
}