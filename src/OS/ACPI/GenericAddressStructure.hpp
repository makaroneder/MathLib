#ifndef GenericAddressStructure_H
#define GenericAddressStructure_H
#include <stdint.h>

struct GenericAddressStructure {
    enum class AddressSpace : uint8_t {
        SystemMemory = 0,
        SystemIO,
        PCIConfigurationSpace,
        EmbeddedController,
        SystemManagementBus,
        SystemCMOS,
        PCIDeviceBARTarget,
        IntelligentPlatformManagementInfrastructure,
        GeneralPurposeIO,
        GenericSerialBus,
        PlatformCommunicationChannel,
    };
    enum class AccessSize : uint8_t {
        Undefined = 0,
        U8Access,
        U16Access,
        U32Access,
        U64Access,
    };
    uint64_t Read(void) const;
    bool Write(uint64_t value);

    AddressSpace addressSpace;
    uint8_t bitWidth;
    uint8_t bitOffset;
    AccessSize accessSize;
    uint64_t address;
} __attribute__((packed));

#endif