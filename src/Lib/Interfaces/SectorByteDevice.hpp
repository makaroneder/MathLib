#ifndef SectorByteDevice_H
#define SectorByteDevice_H
#include "ByteDevice.hpp"

/// @brief Byte device with data stored in sectors
struct SectorByteDevice : ByteDevice {
    /// @brief Returns size of every sector
    /// @return Size of every sector
    virtual size_t GetSectorSize(void) const = 0;
    /// @brief Reads sectors
    /// @param sector First sector to read
    /// @param buff Buffer to read data into
    /// @param count Number of sectors to read
    /// @return Status
    virtual bool ReadSectors(const size_t& sector, void* buff, const size_t& count) = 0;
    /// @brief Writes sectors
    /// @param sector First sector to write
    /// @param buff Buffer to write data to
    /// @param count Number of sectors to write
    /// @return Status
    virtual bool WriteSectors(const size_t& sector, const void* buff, const size_t& count) = 0;
    /// @brief Reads data from the buffer
    /// @param buffer Buffer to read data from
    /// @param size Size of the buffer
    /// @param position Position to read from
    /// @return Size of bytes read
    virtual size_t ReadPositionedSizedBuffer(void* buffer, const size_t& size, const size_t& position) override;
    /// @brief Writes data to the buffer
    /// @param buffer Buffer to write data to
    /// @param size Size of the buffer
    /// @param position Position to write to
    /// @return Size of written bytes
    virtual size_t WritePositionedSizedBuffer(const void* buffer, const size_t& size, const size_t& position) override;
};

#endif