#include "Saveable.hpp"
#include "FileSystem/File.hpp"

namespace MathLib {
    bool Saveable::SaveFromPath(FileSystem& fileSystem, const Sequence<char>& path) const {
        File file = fileSystem.Open(path, OpenMode::Write);
        return Save(file);
    }
    bool Saveable::LoadFromPath(FileSystem& fileSystem, const Sequence<char>& path) {
        File file = fileSystem.Open(path, OpenMode::Read);
        return Load(file);
    }
}