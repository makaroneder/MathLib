#ifndef MathLib_FileSystem_FAT_H
#define MathLib_FileSystem_FAT_H
#include "../PhysicalFileSystem.hpp"
#include "FATDirectoryEntry.hpp"
#include "FATBootSector.hpp"
#include "FATFile.hpp"

namespace MathLib {
    struct FAT : PhysicalFileSystem {
        enum class Type {
            None,
            FAT12,
            FAT16,
            FAT32,
        };
        FAT(ByteDevice& disk);
        [[nodiscard]] virtual bool IsValid(void) const override;
        [[nodiscard]] virtual bool Create(void) override;
        /// @brief Opens file
        /// @param path Path to file
        /// @param mode Mode to open file in
        /// @return File index
        [[nodiscard]] virtual size_t OpenInternal(const Sequence<char>& path, OpenMode mode) override;
        /// @brief Closes file
        /// @param file File index
        /// @return Status
        [[nodiscard]] virtual bool Close(size_t file) override;
        /// @brief Reads data to the buffer
        /// @param file File index
        /// @param buffer Buffer to read data into
        /// @param size Size of the buffer
        /// @param position Position to read from
        /// @return Size of bytes read
        [[nodiscard]] virtual size_t Read(size_t file, void* buffer, size_t size, size_t position) override;
        /// @brief Writes data to the buffer
        /// @param file File index
        /// @param buffer Buffer to write data to
        /// @param size Size of the buffer
        /// @param position Position to write to
        /// @return Size of written bytes
        [[nodiscard]] virtual size_t Write(size_t file, const void* buffer, size_t size, size_t position) override;
        /// @brief Returns file size
        /// @param file File index
        /// @return File size
        [[nodiscard]] virtual size_t GetSize(size_t file) override;
        [[nodiscard]] virtual Array<FileInfo> ReadDirectory(const Sequence<char>& path) override;
        [[nodiscard]] virtual bool CreateDirectory(const Sequence<char>& path, bool overwrite) override;

        private:
        [[nodiscard]] size_t ClusterToSector(size_t cluster) const;
        [[nodiscard]] uint32_t GetNextCluster(size_t cluster, uint8_t fat) const;
        [[nodiscard]] Expected<FATDirectoryEntry> GetDirectoryEntry(const Sequence<char>& path);

        Type type;
        FATBootSector bootSector;
        size_t dataSection;
        size_t root;
        size_t sectorsPerFAT;
        Array<FATFile> files;
    };
}

#endif