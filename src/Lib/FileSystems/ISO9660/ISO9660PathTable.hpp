#ifndef FileSystems_ISO9660_PathTable_H
#define FileSystems_ISO9660_PathTable_H
#include <stdint.h>

namespace MathLib {
    struct ISO9660PathTable {
        uint8_t identifierLength;
        uint8_t extendedAttributeRecordLength;
        uint32_t extent;
        uint16_t parentDirectory;
        char identifier[];
    } __attribute__((packed));
}

#endif