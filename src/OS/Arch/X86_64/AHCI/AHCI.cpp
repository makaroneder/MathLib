#include "AHCI.hpp"
#include "HBAMemory.hpp"
#include "../../../Disks.hpp"
#include "../PCI/PCIHeader0.hpp"

AHCIPort::AHCIPort(void) : port(nullptr), buffer(nullptr) {}
AHCIPort::AHCIPort(HBAPort* port) : port(port), buffer(new uint8_t[1024 + 256 * 33 + 1023]) {
    if (!buffer) Panic("Failed to allocate AHCI buffer");
    port->Configure(buffer + (uintptr_t)buffer % 1024);
    port->GetSize(sectors, bytesPerSector);
}
AHCIPort::~AHCIPort(void) {
    if (buffer) delete [] buffer;
}
size_t AHCIPort::GetSize(void) const {
    return sectors * bytesPerSector;
}
size_t AHCIPort::GetSectorSize(void) const {
    return bytesPerSector;
}
bool AHCIPort::ReadSectors(const size_t& sector, void* buff, const size_t& count) {
    return port->AccessPort(sector, count, buff, false);
}
bool AHCIPort::WriteSectors(const size_t& sector, const void* buff, const size_t& count) {
    return port->AccessPort(sector, count, (void*)buff, true);
}
bool InitAHCI(PCIHeader* header) {
    if (header->type) return false;
    HBAMemory* abar = (HBAMemory*)(uintptr_t)((PCIHeader0*)header)->bar[5];
    for (uint8_t i = 0; i < 32; i++) {
        if (abar->ports[i].IsValid()) {
            AHCIPort* port = new AHCIPort(&abar->ports[i]);
            if (!port || !disks.Add(port)) return false;
        }
    }
    return true;
}