#ifndef MADT_H
#define MADT_H
#include "ACPITable.hpp"

struct MADT : ACPITable {
    static constexpr const char* expectedSignature = "APIC";
    enum class Flags : uint8_t {
        Dual8259 = 0,
    };
    uint32_t localAPIC;
    uint32_t flags;
} __attribute__((packed));
struct MADTEntry {
    enum class Type : uint8_t {
        ProcessorLocalAPIC = 0,
        IOAPIC,
        IOAPICInterruptSourceOverride,
        IOAPICNonMaskableInterruptSource,
        LocalAPICNonMaskableInterrupts,
        LocalAPICAddressOverride,
        ProcessorLocalX2APIC = 9,
    };
    Type type;
    uint8_t length;
} __attribute__((packed));
struct MADTProcessorLocalAPIC : MADTEntry {
    enum class Flags : uint8_t {
        ProcessorEnabled = 0,
        OnlineCapable,
    };
    uint8_t acpiProcessorID;
    uint8_t apicID;
    uint8_t flags;
} __attribute__((packed));
struct MADTIOAPIC : MADTEntry {
    uint8_t ioAPICID;
    uint8_t reserved;
    uint32_t ioAPIC;
    uint32_t gsiBase;
} __attribute__((packed));
struct MADTIOAPICInterruptSourceOverride : MADTEntry {
    uint8_t busSource;
    uint8_t irqSource;
    uint32_t gsi;
    uint16_t flags;
} __attribute__((packed));
struct MADTIOAPICNonMaskableInterruptSource : MADTEntry {
    uint8_t nmiSource;
    uint8_t reserved;
    uint16_t flags;
    uint32_t gsi;
} __attribute__((packed));
struct MADTLocalAPICNonMaskableInterrupts : MADTEntry {
    uint8_t acpiProcessorID;
    uint16_t flags;
    uint8_t lint;
} __attribute__((packed));
struct MADTLocalAPICAddressOverride : MADTEntry {
    uint16_t reserved;
    uint64_t address;
} __attribute__((packed));
struct MADTProcessorLocalX2APIC : MADTEntry {
    uint16_t reserved;
    uint32_t localX2APICID;
    uint32_t flags;
    uint32_t acpiID;
} __attribute__((packed));

#endif