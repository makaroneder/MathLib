#ifndef UHCI_H
#define UHCI_H
#include "../PCI/PCIHeader.hpp"

struct UHCIStatusRegister {
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
struct UHCI {
    UHCI(PCIHeader* header);
};

#endif