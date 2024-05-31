#ifndef Disk_H
#define Disk_H
#include "SeekMode.hpp"
#include <string>

/// @brief Disk
struct Disk {
    /// @brief Virtual destructor
    virtual ~Disk(void);
    /// @brief Returns size of every disk sector
    /// @return Size of every disk sector
    virtual size_t GetSectorSize(void) const = 0;
    /// @brief Returns disk size
    /// @return Disk size
    virtual size_t GetSize(void) const = 0;
    /// @brief Updates disk
    /// @return Status
    virtual bool Flush(void) = 0;
    /// @brief Reads data from disk
    /// @param buffer Buffer to read data into
    /// @param size Size of buffer
    /// @return Status
    bool Read(void* buffer, size_t size);
    /// @brief Writes data to disk
    /// @param buffer Buffer to write data from
    /// @param size Size of buffer
    /// @return Status
    bool Write(const void* buffer, size_t size);
    /// @brief Sets disk position
    /// @param offset Offset to set
    /// @param mode Mode to use when setting position
    /// @return Status
    bool Seek(ssize_t offset, SeekMode mode);
    /// @brief Returns current position
    /// @return Current position
    size_t Tell(void);

    protected:
    /// @brief Position of disk
    size_t position;

    /// @brief Reads sectors from disk
    /// @param sector First sector to read
    /// @param buff Buffer to read data into
    /// @param count Number of sectors to read
    /// @return Status
    virtual bool ReadSectors(size_t sector, void* buff, size_t count) = 0;
    /// @brief Writes sectors to disk
    /// @param sector First sector to write
    /// @param buff Buffer to write data from
    /// @param count Number of sectors to write
    /// @return Status
    virtual bool WriteSectors(size_t sector, const void* buff, size_t count) = 0;
};

#endif