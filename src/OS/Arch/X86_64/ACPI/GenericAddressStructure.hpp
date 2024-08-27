#ifndef GenericAddressStructure_H
#define GenericAddressStructure_H
#include <Expected.hpp>
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
    Expected<uint64_t> Read(const AccessSize& legacySize = AccessSize::Undefined) const;
    bool Write(uint64_t value, const AccessSize& legacySize = AccessSize::Undefined);

    AddressSpace addressSpace;
    uint8_t bitWidth;
    uint8_t bitOffset;
    AccessSize accessSize;
    uint64_t address;
} __attribute__((packed));

#endif