#include "File.hpp"
#include "Host.hpp"

File::File(FileSystem& fileSystem, size_t index) : fileSystem(fileSystem), index(index) {
    if (index == SIZE_MAX) Panic("Invalid file index");
}
File::~File(void) {
    if (!fileSystem.Close(index)) Panic("Failed to close file");
}
bool File::ReadBuffer(void* buffer, const size_t& size) {
    return fileSystem.Read(index, buffer, size);
}
bool File::WriteBuffer(const void* buffer, const size_t& size) {
    return fileSystem.Write(index, buffer, size);
}
bool File::Seek(const ssize_t& offset, const SeekMode& mode) {
    return fileSystem.Seek(index, offset, mode);
}
size_t File::Tell(void) const {
    return fileSystem.Tell(index);
}
size_t File::GetSize(void) const {
    return fileSystem.GetSize(index);
}