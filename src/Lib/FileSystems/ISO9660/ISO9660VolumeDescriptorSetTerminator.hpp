#ifndef FileSystems_ISO9660_VolumeDescriptorSetTerminator_H
#define FileSystems_ISO9660_VolumeDescriptorSetTerminator_H
#include "ISO9660VolumeDescriptor.hpp"

namespace MathLib {
    struct ISO9660VolumeDescriptorSetTerminator : ISO9660VolumeDescriptor {
        uint8_t reserved[2041];
    } __attribute__((packed));
}

#endif