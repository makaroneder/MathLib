#ifdef __x86_64__
#include "UHCI.hpp"
#include "../PCI/PCIHeader0.hpp"
#include <Host.hpp>

UHCI::UHCI(PCIHeader* header) {
    if (header->type) Panic("Invalid header type");
    base = ((UHCIConfigurationSpace*)(uintptr_t)((PCIHeader0*)header)->bar[4])->ioBase;
    for (uint8_t i = 0; i <= 2; i += 2) {
        UHCIStatusRegister port = Read<uint16_t>(0x10 + i);
        port.reset = true;
        Write<uint16_t>(0x10 + i, port.value);
        for (size_t i = 0; i < 1000; i++) {}
        port.reset = false;
        Write<uint16_t>(0x10 + i, port.value);
    }
    // TODO:
}

#endif