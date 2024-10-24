#ifdef __x86_64__
#include "AHCI.hpp"
#include "HBAMemory.hpp"
#include "../../../Disks.hpp"
#include "../PCI/PCIHeader0.hpp"
#include <Logger.hpp>
#include <String.hpp>

AHCIPort::AHCIPort(void) : port(nullptr), buffer(nullptr) {}
AHCIPort::AHCIPort(HBAPort* port) : port(port), buffer(new uint8_t[1024 + 256 * 33 + 1023]) {
    if (!buffer) Panic("Failed to allocate AHCI buffer");
    port->Configure(buffer + (uintptr_t)buffer % 1024);
    if (!port->GetSize(sectors, bytesPerSector)) Panic("Failed to get size of AHCI");
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
bool AHCIPort::ReadSectors(size_t sector, void* buff, size_t count) {
    return port->AccessPort(sector, count, buff, false);
}
bool AHCIPort::WriteSectors(size_t sector, const void* buff, size_t count) {
    return port->AccessPort(sector, count, (void*)buff, true);
}
bool InitAHCI(PCIHeader* header) {
    if (header->type) return false;
    HBAMemory* abar = (HBAMemory*)(uintptr_t)((PCIHeader0*)header)->bar[5];
    for (uint8_t i = 0; i < 32; i++) {
        if (abar->ports[i].IsValid()) {
            const HBAPort::Type tmp = abar->ports[i].GetType();
            String type;
            switch (tmp) {
                case HBAPort::Type::ATAPI: {
                    type = "ATAPI";
                    break;
                }
                case HBAPort::Type::ATA: {
                    type = "ATA";
                    break;
                }
                case HBAPort::Type::SEMB: {
                    type = "SEMB";
                    break;
                }
                case HBAPort::Type::PM: {
                    type = "PM";
                    break;
                }
                default: type = String("Unknown (0x") + ToString((uint32_t)tmp, 16) + ')';
            }
            LogString(String("Found ") + type + " on AHCI port " + ToString(i) + '\n');
            AHCIPort* port = new AHCIPort(&abar->ports[i]);
            if (!port || !disks.Add(port)) return false;
        }
    }
    return true;
}

#endif