#ifndef MathLib_FileSystem_ISO9660_PathTableLocation_H
#define MathLib_FileSystem_ISO9660_PathTableLocation_H
#include <stdint.h>

namespace MathLib {
    struct ISO9660PathTableLocation {
        uint32_t location;
        uint32_t optionalLocation;
    } __attribute__((packed));
}

#endif