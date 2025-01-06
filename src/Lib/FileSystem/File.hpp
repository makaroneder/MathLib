#ifndef MathLib_FileSystem_File_H
#define MathLib_FileSystem_File_H
#include "FileSystem.hpp"
#include "../Interfaces/ByteDevice.hpp"

namespace MathLib {
    struct File : ByteDevice {
        File(FileSystem& fileSystem, size_t index);
        virtual ~File(void) override;
        /// @brief Returns file size
        /// @return File size
        [[nodiscard]] virtual size_t GetSize(void) const override;
        /// @brief Reads data from the buffer
        /// @param buffer Buffer to read data from
        /// @param size Size of the buffer
        /// @param position Position to read from
        /// @return Size of bytes read
        [[nodiscard]] virtual size_t ReadPositionedSizedBuffer(void* buffer, size_t size, size_t position) override;
        /// @brief Writes data to the buffer
        /// @param buffer Buffer to write data to
        /// @param size Size of the buffer
        /// @param position Position to write to
        /// @return Size of written bytes
        [[nodiscard]] virtual size_t WritePositionedSizedBuffer(const void* buffer, size_t size, size_t position) override;

        private:
        FileSystem& fileSystem;
        size_t index;
    };
}

#endif