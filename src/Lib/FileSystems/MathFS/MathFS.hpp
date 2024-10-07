#ifndef FileSystems_MathFS_H
#define FileSystems_MathFS_H
#include "../PhysicalFileSystem.hpp"

struct MathFS : PhysicalFileSystem {
    MathFS(ByteDevice& disk);
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
    uint64_t root;
};

#endif