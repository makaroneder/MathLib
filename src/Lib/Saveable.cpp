#include "Saveable.hpp"

Saveable::~Saveable(void) {}
bool Saveable::SaveFromPath(FileSystem& fileSystem, String path) const {
    const size_t file = fileSystem.Open(path, OpenMode::WriteByte);
    return file != SIZE_MAX && Save(fileSystem, file) && fileSystem.Flush(file) && fileSystem.Close(file);
}
bool Saveable::LoadFromPath(FileSystem& fileSystem, String path) {
    const size_t file = fileSystem.Open(path, OpenMode::ReadByte);
    return file != SIZE_MAX && Load(fileSystem, file) && fileSystem.Flush(file) && fileSystem.Close(file);
}