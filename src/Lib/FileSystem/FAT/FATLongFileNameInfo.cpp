#include "FATLongFileNameInfo.hpp"
#include "../../Typedefs.hpp"

namespace MathLib {
    FATLongFileNameInfo::FATLongFileNameInfo(void) : order(UINT8_MAX) {}
    FATLongFileNameInfo::FATLongFileNameInfo(const FATLongFileName& lfn) : order(lfn.order) {
        for (size_t i = 0; i < SizeOfArray(lfn.name1); i++) name[i] = lfn.name1[i];
        for (size_t i = 0; i < SizeOfArray(lfn.name2); i++) name[i + SizeOfArray(lfn.name1)] = lfn.name2[i];
        for (size_t i = 0; i < SizeOfArray(lfn.name3); i++) name[i + SizeOfArray(lfn.name1) + SizeOfArray(lfn.name2)] = lfn.name3[i];
    }
    bool FATLongFileNameInfo::LessThanEqual(const Orderable& other) const {
        return order <= ((const FATLongFileNameInfo&)other).order;
    }
}