#ifndef Freestanding
#ifndef HostDisk_H
#define HostDisk_H
#include "../Disk.hpp"
#include "../Typedefs.hpp"

/// @brief Virtual disk using host file
struct HostDisk : Disk {
    /// @brief Destroys disk
    virtual ~HostDisk(void) override;
    /// @brief Creates a new disk
    /// @param path Path of file to use as disk
    /// @param size_ Disk size
    /// @param sectorSize_ Sector size
    HostDisk(String path, size_t size_, size_t sectorSize_);
    /// @brief Returns size of every disk sector
    /// @return Size of every disk sector
    virtual size_t GetSectorSize(void) const override;
    /// @brief Returns disk size
    /// @return Disk size
    virtual size_t GetSize(void) const override;
    /// @brief Updates disk
    /// @return Status
    virtual bool Flush(void) override;

    protected:
    /// @brief Reads sectors from disk
    /// @param sector First sector to read
    /// @param buff Buffer to read data into
    /// @param count Number of sectors to read
    /// @return Status
    virtual bool ReadSectors(size_t sector, void* buff, size_t count) override;
    /// @brief Writes sectors to disk
    /// @param sector First sector to write
    /// @param buff Buffer to write data from
    /// @param count Number of sectors to write
    /// @return Status
    virtual bool WriteSectors(size_t sector, const void* buff, size_t count) override;

    private:
    /// @brief Disk size
    size_t size;
    /// @brief Sector size
    size_t sectorSize;
    /// @brief File to use as disk
    FILE* file;
};

#endif
#endif