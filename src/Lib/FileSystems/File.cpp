#include "File.hpp"
#include "../Host.hpp"

File::File(FileSystem& fileSystem, size_t index) : fileSystem(fileSystem), index(index) {
    if (index == SIZE_MAX) Panic("Invalid file index");
}
File::~File(void) {
    if (!fileSystem.Close(index)) Panic("Failed to close file");
}
size_t File::ReadPositionedSizedBuffer(void* buffer, const size_t& size, const size_t& position) {
    return fileSystem.Read(index, buffer, size, position);
}
size_t File::WritePositionedSizedBuffer(const void* buffer, const size_t& size, const size_t& position) {
    return fileSystem.Write(index, buffer, size, position);
}
size_t File::GetSize(void) const {
    return fileSystem.GetSize(index);
}