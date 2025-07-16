#ifdef __x86_64__
#ifndef UHCI_H
#define UHCI_H
#include "../PCI/PCIHeader.hpp"
#include "../IO.hpp"

struct UHCIConfigurationSpace {
    uint64_t reserved1;
    uint8_t reserved2;
    uint32_t classCode : 24;
    uint32_t reserved3 : 20;
    uint32_t ioBase;
    uint64_t reserved4 : 60;
    uint8_t serialBusReleaseNumber;
    uint8_t reserved5[228];
} __attribute__((packed));
union UHCIStatusRegister {
    uint16_t value;
    struct {
        bool connected : 1;
        bool connectedChanged : 1;
        bool portEnabled : 1;
        bool portEnableChanged : 1;
        uint8_t lineStatus : 2;
        bool resumeDetected : 1;
        uint8_t reserved3 : 1;
        bool lowSpeed : 1;
        bool reset : 1;
        uint8_t reserved2 : 2;
        bool suspend : 1;
        uint8_t reserved1 : 3;
    } __attribute__((packed));

    UHCIStatusRegister(uint16_t value) : value(value) {}
} __attribute__((packed));
struct UHCI {
    UHCI(PCIHeader* header);

    private:
    template <typename T>
    [[nodiscard]] T Read(uint8_t offset) const {
        return ReadPort<T>(base + offset);
    }
    template <typename T>
    void Write(uint8_t offset, T value) {
        WritePort<T>(base + offset, value);
    }

    uint32_t base;
};

#endif
#endif