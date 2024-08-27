#ifndef VolumeDescriptorSetTerminator_H
#define VolumeDescriptorSetTerminator_H
#include "VolumeDescriptor.hpp"

struct VolumeDescriptorSetTerminator : VolumeDescriptor {
    uint8_t reserved[2041];
} __attribute__((packed));

#endif