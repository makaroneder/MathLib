#ifndef Disk_H
#define Disk_H
#include "PositionedByteDevice.hpp"

/// @brief Disk
struct Disk : PositionedByteDevice {
    /// @brief Returns size of every disk sector
    /// @return Size of every disk sector
    virtual size_t GetSectorSize(void) const = 0;
    /// @brief Reads data from disk
    /// @param buffer Buffer to read data into
    /// @param size Size of buffer
    /// @return Status
    virtual bool ReadBuffer(void* buffer, const size_t& size) override;
    /// @brief Writes data to disk
    /// @param buffer Buffer to write data from
    /// @param size Size of buffer
    /// @return Status
    virtual bool WriteBuffer(const void* buffer, const size_t& size) override;

    protected:
    /// @brief Reads sectors from disk
    /// @param sector First sector to read
    /// @param buff Buffer to read data into
    /// @param count Number of sectors to read
    /// @return Status
    virtual bool ReadSectors(const size_t& sector, void* buff, const size_t& count) = 0;
    /// @brief Writes sectors to disk
    /// @param sector First sector to write
    /// @param buff Buffer to write data from
    /// @param count Number of sectors to write
    /// @return Status
    virtual bool WriteSectors(const size_t& sector, const void* buff, const size_t& count) = 0;
};

#endif