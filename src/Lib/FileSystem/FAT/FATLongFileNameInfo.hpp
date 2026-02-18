#ifndef MathLib_FileSystem_FAT_LongFileNameInfo_H
#define MathLib_FileSystem_FAT_LongFileNameInfo_H
#include "FATLongFileName.hpp"
#include <Interfaces/Orderable.hpp>

namespace MathLib {
    struct FATLongFileNameInfo : Orderable {
        uint8_t order;
        char16_t name[13];

        FATLongFileNameInfo(void);
        FATLongFileNameInfo(const FATLongFileName& lfn);

        protected:
        [[nodiscard]] virtual bool LessThanEqual(const Orderable& other) const override;
    };
}

#endif