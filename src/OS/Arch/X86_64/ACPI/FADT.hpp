#ifndef FADT_H
#define FADT_H
#include "ACPITable.hpp"
#include "GenericAddressStructure.hpp"

struct FADT : ACPITable {
    static constexpr const char* expectedSignature = "FACP";
    enum class PreferredPowerManagementProfile : uint8_t {
        Unspecified = 0,
        Desktop,
        Mobile,
        Workstation,
        EnterpriseServer,
        SOHOServer,
        AplliancePC,
        PerformanceServer,
    };

    uint32_t facs;
    uint32_t dsdt;
    uint8_t reserved;
    PreferredPowerManagementProfile preferredPowerManagementProfile;
    uint16_t sciInterrupt;
    uint32_t smiCommandPort;
    uint8_t acpiEnable;
    uint8_t acpiDisable;
    uint8_t s4BIOSReq;
    uint8_t pStateControl;
    uint32_t pm1aEventBlock;
    uint32_t pm1bEventBlock;
    uint32_t pm1aControlBlock;
    uint32_t pm1bControlBlock;
    uint32_t pm2ControlBlock;
    uint32_t pmTimerBlock;
    uint32_t gpe0Block;
    uint32_t gpe1Block;
    uint8_t pm1EventLength;
    uint8_t pm1ControlLength;
    uint8_t pm2ControlLength;
    uint8_t pmTimerLength;
    uint8_t gpe0Length;
    uint8_t gpe1Length;
    uint8_t gpe1Base;
    uint8_t cStateControl;
    uint16_t worstC2Latency;
    uint16_t worstC3Latency;
    uint16_t flushSize;
    uint16_t flushStride;
    uint8_t dutyOffset;
    uint8_t dutyWidth;
    uint8_t dayAlarm;
    uint8_t monthAlarm;
    uint8_t rtcCenturyRegister;
    uint16_t bootArchitectureFlags;
} __attribute__((packed));
struct FADT2 : FADT {
    uint8_t reserved1;
    uint32_t flags;
    GenericAddressStructure resetRegister;
    uint8_t resetValue;
    uint8_t reserved2[3];
    uint64_t facs2;
    uint64_t dsdt2;
    GenericAddressStructure pm1aEventBlock2;
    GenericAddressStructure pm1bEventBlock2;
    GenericAddressStructure pm1aControlBlock2;
    GenericAddressStructure pm1bControlBlock2;
    GenericAddressStructure pm2ControlBlock2;
    GenericAddressStructure pmTimerBlock2;
    GenericAddressStructure gpe0Block2;
    GenericAddressStructure gpe1Block2;
} __attribute__((packed));

#endif