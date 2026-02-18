#ifndef MathLib_FileSystem_ISO9660_VolumeDescriptorSetTerminator_H
#define MathLib_FileSystem_ISO9660_VolumeDescriptorSetTerminator_H
#include "ISO9660VolumeDescriptor.hpp"

namespace MathLib {
    struct ISO9660VolumeDescriptorSetTerminator : ISO9660VolumeDescriptor {
        ISO9660VolumeDescriptorSetTerminator(void);

        uint8_t reserved[2041];
    } __attribute__((packed));
}

#endif