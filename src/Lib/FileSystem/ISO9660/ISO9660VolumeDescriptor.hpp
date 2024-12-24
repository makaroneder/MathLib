#ifndef MathLib_FileSystem_ISO9660_VolumeDescriptor_H
#define MathLib_FileSystem_ISO9660_VolumeDescriptor_H
#include <stdint.h>

namespace MathLib {
    struct ISO9660VolumeDescriptor {
        static constexpr const char* expectedSignature = "CD001";
        static constexpr uint8_t expectedVersion = 0x01;
        enum class Type : uint8_t {
            BootRecord = 0,
            PrimaryVolumeDescriptor,
            SupplementaryVolumeDescriptor,
            VolumePartitionDescriptor,
            VolumeDescriptorSetTerminator = UINT8_MAX,
        } type;
        char signature[5];
        uint8_t version;

        bool IsValid(void) const;
    } __attribute__((packed));
}

#endif