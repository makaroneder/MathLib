#include "ISO9660DirectoryEntryDate.hpp"
#include "../../Benchmark.hpp"

namespace MathLib {
    bool ISO9660DirectoryEntryDate::IsValid(void) const {
        StartAndReturnFromBenchmark(!(
            month > 12 || !month ||
            day > 31 || !day ||
            hour > 23 ||
            minute > 59 ||
            second > 59 ||
            timeZoneOffset > 100
        ));
    }
}