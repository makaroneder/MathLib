#include "FATLongFileNameInfo.hpp"
#include "../../Typedefs.hpp"

namespace MathLib {
    FATLongFileNameInfo::FATLongFileNameInfo(void) : order(UINT8_MAX) {
        EmptyBenchmark
    }
    FATLongFileNameInfo::FATLongFileNameInfo(const FATLongFileName& lfn) : order(lfn.order) {
        StartBenchmark
        for (size_t i = 0; i < SizeOfArray(lfn.name1); i++) name[i] = lfn.name1[i];
        for (size_t i = 0; i < SizeOfArray(lfn.name2); i++) name[i + SizeOfArray(lfn.name1)] = lfn.name2[i];
        for (size_t i = 0; i < SizeOfArray(lfn.name3); i++) name[i + SizeOfArray(lfn.name1) + SizeOfArray(lfn.name2)] = lfn.name3[i];
        EndBenchmark
    }
    bool FATLongFileNameInfo::operator<(const FATLongFileNameInfo& other) const {
        StartAndReturnFromBenchmark(order < other.order);
    }
}