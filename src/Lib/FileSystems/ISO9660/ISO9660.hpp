#ifndef ISO9660_H
#define ISO9660_H
#include "ISO9660File.hpp"
#include "../PhysicalFileSystem.hpp"
#include "PrimaryVolumeDescriptor.hpp"

// TODO: Add extensions like SUSP, Rock Ridge, El Torito, Joilet, Romeo, etc.
struct ISO9660 : PhysicalFileSystem {
    ISO9660(ByteDevice& disk);
    bool IsValid(void) const;
    virtual bool Create(void) override;
    /// @brief Opens file
    /// @param path Path to file
    /// @param mode Mode to open file in
    /// @return File index
    virtual size_t OpenInternal(const String& path, const OpenMode& mode) override;
    /// @brief Closes file
    /// @param file File index
    /// @return Status
    virtual bool Close(const size_t& file) override;
    /// @brief Reads data to the buffer
    /// @param file File index
    /// @param buffer Buffer to read data into
    /// @param size Size of the buffer
    /// @param position Position to read from
    /// @return Size of bytes read
    virtual size_t Read(const size_t& file, void* buffer, const size_t& size, const size_t& position) override;
    /// @brief Writes data to the buffer
    /// @param file File index
    /// @param buffer Buffer to write data to
    /// @param size Size of the buffer
    /// @param position Position to write to
    /// @return Size of written bytes
    virtual size_t Write(const size_t& file, const void* buffer, const size_t& size, const size_t& position) override;
    /// @brief Returns file size
    /// @param file File index
    /// @return File size
    virtual size_t GetSize(const size_t& file) override;
    virtual Array<FileInfo> ReadDirectory(const String& path) override;

    private:
    Array<DirectoryEntry*> ReadDirectoryEntry(const DirectoryEntry& parent);
    Expected<DirectoryEntry> GetDirectoryEntry(const String& path);

    PrimaryVolumeDescriptor pvd;
    Array<ISO9660File> files;
};

#endif