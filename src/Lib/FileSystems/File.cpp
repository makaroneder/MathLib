#include "File.hpp"
#include "../Host.hpp"

namespace MathLib {
    File::File(FileSystem& fileSystem, size_t index) : fileSystem(fileSystem), index(index) {}
    File::~File(void) {
        if (index != SIZE_MAX && !fileSystem.Close(index)) Panic("Failed to close file");
    }
    size_t File::ReadPositionedSizedBuffer(void* buffer, size_t size, size_t position) {
        return index == SIZE_MAX ? 0 : fileSystem.Read(index, buffer, size, position);
    }
    size_t File::WritePositionedSizedBuffer(const void* buffer, size_t size, size_t position) {
        return index == SIZE_MAX ? 0 : fileSystem.Write(index, buffer, size, position);
    }
    size_t File::GetSize(void) const {
        return index == SIZE_MAX ? 0 : fileSystem.GetSize(index);
    }
}