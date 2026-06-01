#ifndef MathLib_FileSystem_File_H
#define MathLib_FileSystem_File_H
#include "../Interfaces/ByteDevice.hpp"

namespace MathLib {
    struct FileSystem;
    struct File : ByteDevice {
        File(FileSystem& fileSystem, size_t index);
        virtual ~File(void) override;
        bool Exists(void) const;
        [[nodiscard]] virtual size_t GetSize(void) const override;
        [[nodiscard]] virtual size_t ReadPositionedSizedBuffer(void* buffer, size_t size, size_t position) override;
        [[nodiscard]] virtual size_t WritePositionedSizedBuffer(const void* buffer, size_t size, size_t position) override;

        private:
        FileSystem& fileSystem;
        size_t index;
    };
}

#endif