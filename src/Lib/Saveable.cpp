#include "Saveable.hpp"

bool Saveable::SaveFromPath(FileSystem& fileSystem, const String& path) const {
    File file = fileSystem.Open(path, OpenMode::Write);
    return Save(file);
}
bool Saveable::LoadFromPath(FileSystem& fileSystem, const String& path) {
    File file = fileSystem.Open(path, OpenMode::Read);
    return Load(file);
}