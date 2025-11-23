#ifndef MathLib_FileSystem_ISO9660_PathTable_H
#define MathLib_FileSystem_ISO9660_PathTable_H
#include <stdint.h>

namespace MathLib {
    struct ISO9660PathTable {
        uint8_t identifierLength;
        uint8_t extendedAttributeRecordLength;
        uint32_t extent;
        uint16_t parentDirectory;
        char identifier[];

        ISO9660PathTable(void);
    } __attribute__((packed));
}

#endif