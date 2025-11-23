#ifndef MathLib_FileSystem_ISO9660_SUSPHeader_H
#define MathLib_FileSystem_ISO9660_SUSPHeader_H
#include <stdint.h>

namespace MathLib {
    struct SUSPHeader {
        char signature[2];
        uint8_t length;
        uint8_t version;

        SUSPHeader(void);
    } __attribute__((packed));
}

#endif