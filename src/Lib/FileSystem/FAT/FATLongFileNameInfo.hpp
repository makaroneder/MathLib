#ifndef MathLib_FileSystem_FAT_LongFileNameInfo_H
#define MathLib_FileSystem_FAT_LongFileNameInfo_H
#include "FATLongFileName.hpp"
#include <Interfaces/Orderable.hpp>

namespace MathLib {
    struct FATLongFileNameInfo;
    struct FATLongFileNameInfo : Orderable<FATLongFileNameInfo> {
        uint8_t order;
        char16_t name[13];

        FATLongFileNameInfo(void);
        FATLongFileNameInfo(const FATLongFileName& lfn);
        [[nodiscard]] virtual bool LessThanEqual(const FATLongFileNameInfo& other) const override;
    };
}

#endif