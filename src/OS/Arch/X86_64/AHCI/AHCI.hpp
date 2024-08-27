#ifndef AHCI_H
#define AHCI_H
#include "HBAPort.hpp"
#include "../PCI/PCIHeader.hpp"
#include <Interfaces/SectorByteDevice.hpp>

struct AHCIPort : SectorByteDevice {
    AHCIPort(void);
    AHCIPort(HBAPort* port);
    virtual ~AHCIPort(void) override;
    /// @brief Returns size
    /// @return Size
    virtual size_t GetSize(void) const override;
    /// @brief Returns size of every sector
    /// @return Size of every sector
    virtual size_t GetSectorSize(void) const override;
    /// @brief Reads sectors
    /// @param sector First sector to read
    /// @param buff Buffer to read data into
    /// @param count Number of sectors to read
    /// @return Status
    virtual bool ReadSectors(const size_t& sector, void* buff, const size_t& count) override;
    /// @brief Writes sectors
    /// @param sector First sector to write
    /// @param buff Buffer to write data to
    /// @param count Number of sectors to write
    /// @return Status
    virtual bool WriteSectors(const size_t& sector, const void* buff, const size_t& count) override;

    private:
    HBAPort* port;
    uint8_t* buffer;
    size_t sectors;
    size_t bytesPerSector;
};
bool InitAHCI(PCIHeader* header);

#endif