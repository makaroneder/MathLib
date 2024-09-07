#include "UHCI.hpp"
#include "../PCI/PCIHeader0.hpp"
#include <Host.hpp>

UHCI::UHCI(PCIHeader* header) {
    if (header->type) Panic("Invalid header type");
    uint32_t* base = (uint32_t*)(uintptr_t)((PCIHeader0*)header)->bar[4];
    UHCIStatusRegister* port1 = (UHCIStatusRegister*)&base[0x10];
    UHCIStatusRegister* port2 = (UHCIStatusRegister*)&base[0x12];
    port1->reset = true;
    for (size_t i = 0; i < 1000; i++) {}
    port1->reset = false;
    port2->reset = true;
    for (size_t i = 0; i < 1000; i++) {}
    port2->reset = false;
    // TODO:
}