#ifndef MathLib_FileSystem_FAT_LongFileNameInfo_H
#define MathLib_FileSystem_FAT_LongFileNameInfo_H
#include "FATLongFileName.hpp"

namespace MathLib {
    struct FATLongFileNameInfo {
        uint8_t order;
        char16_t name[13];

        FATLongFileNameInfo(void);
        FATLongFileNameInfo(const FATLongFileName& lfn);
        bool operator<(const FATLongFileNameInfo& other) const;
    };
}

#endif