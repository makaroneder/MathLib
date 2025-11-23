#ifdef __x86_64__
#ifndef PCIHeader_H
#define PCIHeader_H
#include <stdint.h>

struct PCIHeader {
    enum class DevselTiming : uint8_t {
        Fast = 0,
        Medium,
        Slow,
    };
    uint16_t vendorID;
    uint16_t deviceID;
    uint8_t reserved1 : 5;
    bool interruptDisable : 1;
    bool fastBackToBackEnable : 1;
    bool serrEnable : 1;
    bool reserved2 : 1;
    bool parityErrorResponse : 1;
    bool vgaPaletteSnoop : 1;
    bool memoryWriteAndInvalidateEnable : 1;
    bool specialCycles : 1;
    bool busMaster : 1;
    bool memorySpace : 1;
    bool ioSpace : 1;
    bool parityError : 1;
    bool signaledSystemError : 1;
    bool masterAbort : 1;
    bool targetAbort : 1;
    bool signaledTargetAbort : 1;
    uint8_t devselTiming : 2;
    bool masterDataParityError : 1;
    bool fastBackToBackCapable : 1;
    bool reserved3 : 1;
    bool capableOf66Mhz : 1;
    bool capabilitiesList : 1;
    bool interruptStatus : 1;
    uint8_t reserved4 : 2;
    uint8_t revisionID;
    uint8_t programInterface;
    uint8_t subClass;
    uint8_t classCode;
    uint8_t cacheLineSize;
    uint8_t latencyTimer;
    uint8_t type : 7;
    bool multipleFunctions : 1;
    uint8_t completionCode : 3;
    uint8_t reserved5 : 2;
    uint8_t startBIST : 1;
    bool bistCapable : 1;

    [[nodiscard]] bool IsValid(void) const;
} __attribute__((packed));

#endif
#endif