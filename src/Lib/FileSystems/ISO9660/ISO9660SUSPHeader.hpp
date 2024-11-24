#ifndef FileSystems_ISO9660_SUSPHeader_H
#define FileSystems_ISO9660_SUSPHeader_H
#include <stdint.h>

namespace MathLib {
    struct SUSPHeader {
        char signature[2];
        uint8_t length;
        uint8_t version;
    } __attribute__((packed));
}

#endif