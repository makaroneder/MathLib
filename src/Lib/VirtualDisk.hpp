#ifndef VirtualDisk_H
#define VirtualDisk_H
#include "Disk.hpp"
#include "File.hpp"

/// @brief Virtual disk using file
struct VirtualDisk : Disk {
    /// @brief Creates a new disk
    /// @param file File to use as disk
    /// @param size_ Disk size
    /// @param sectorSize_ Sector size
    VirtualDisk(File& file, const size_t& size_, const size_t& sectorSize_);
    /// @brief Returns size of every disk sector
    /// @return Size of every disk sector
    virtual size_t GetSectorSize(void) const override;
    /// @brief Returns disk size
    /// @return Disk size
    virtual size_t GetSize(void) const override;

    protected:
    /// @brief Reads sectors from disk
    /// @param sector First sector to read
    /// @param buff Buffer to read data into
    /// @param count Number of sectors to read
    /// @return Status
    virtual bool ReadSectors(const size_t& sector, void* buff, const size_t& count) override;
    /// @brief Writes sectors to disk
    /// @param sector First sector to write
    /// @param buff Buffer to write data from
    /// @param count Number of sectors to write
    /// @return Status
    virtual bool WriteSectors(const size_t& sector, const void* buff, const size_t& count) override;

    private:
    /// @brief Disk size
    size_t size;
    /// @brief Sector size
    size_t sectorSize;
    /// @brief File to use as disk
    File& file;
};

#endif