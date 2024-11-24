#ifndef FileSystems_ISO9660_H
#define FileSystems_ISO9660_H
#include "ISO9660File.hpp"
#include "../PhysicalFileSystem.hpp"
#include "ISO9660PrimaryVolumeDescriptor.hpp"

namespace MathLib {
    // TODO: Add extensions like SUSP, Rock Ridge, El Torito, Joilet, Romeo, etc.
    struct ISO9660 : PhysicalFileSystem {
        ISO9660(ByteDevice& disk);
        bool IsValid(void) const;
        virtual bool Create(void) override;
        /// @brief Opens file
        /// @param path Path to file
        /// @param mode Mode to open file in
        /// @return File index
        virtual size_t OpenInternal(const String& path, OpenMode mode) override;
        /// @brief Closes file
        /// @param file File index
        /// @return Status
        virtual bool Close(size_t file) override;
        /// @brief Reads data to the buffer
        /// @param file File index
        /// @param buffer Buffer to read data into
        /// @param size Size of the buffer
        /// @param position Position to read from
        /// @return Size of bytes read
        virtual size_t Read(size_t file, void* buffer, size_t size, size_t position) override;
        /// @brief Writes data to the buffer
        /// @param file File index
        /// @param buffer Buffer to write data to
        /// @param size Size of the buffer
        /// @param position Position to write to
        /// @return Size of written bytes
        virtual size_t Write(size_t file, const void* buffer, size_t size, size_t position) override;
        /// @brief Returns file size
        /// @param file File index
        /// @return File size
        virtual size_t GetSize(size_t file) override;
        virtual Array<FileInfo> ReadDirectory(const String& path) override;

        private:
        Array<ISO9660DirectoryEntry*> ReadDirectoryEntry(const ISO9660DirectoryEntry& parent);
        Expected<ISO9660DirectoryEntry> GetDirectoryEntry(const String& path);

        ISO9660PrimaryVolumeDescriptor pvd;
        Array<ISO9660File> files;
    };
}

#endif