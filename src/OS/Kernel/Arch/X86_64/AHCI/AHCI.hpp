#ifdef __x86_64__
#ifndef AHCI_H
#define AHCI_H
#include "HBAPort.hpp"
#include "../PCI/PCIHeader.hpp"
#include <Interfaces/SectorByteDevice.hpp>

struct AHCIPort : MathLib::SectorByteDevice {
    AHCIPort(void);
    AHCIPort(HBAPort* port);
    virtual ~AHCIPort(void) override;
    [[nodiscard]] virtual size_t GetSize(void) const override;
    [[nodiscard]] virtual size_t GetSectorSize(void) const override;
    [[nodiscard]] virtual bool ReadSectors(size_t sector, void* buff, size_t count) override;
    [[nodiscard]] virtual bool WriteSectors(size_t sector, const void* buff, size_t count) override;

    private:
    HBAPort* port;
    uint8_t* buffer;
    size_t sectors;
    size_t bytesPerSector;
};
bool InitAHCI(PCIHeader* header);

#endif
#endif