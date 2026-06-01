#include "Saveable.hpp"
#include "FileSystem/File.hpp"
#include "Interfaces/Sequence/SequenceReadable.hpp"

namespace MathLib {
    bool Saveable::SaveFromPath(FileSystem& fileSystem, const Sequence<char>& path) const {
        File file = fileSystem.Open(path, OpenMode::Write);
        return file.Exists() && Save(file);
    }
    bool Saveable::LoadFromPath(FileSystem& fileSystem, const Sequence<char>& path) {
        File file = fileSystem.Open(path, OpenMode::Read);
        return file.Exists() && Load(file);
    }
    bool Saveable::SaveFromPath(FileSystem* fileSystem, const Sequence<char>& path) const {
        return fileSystem && SaveFromPath(*fileSystem, path);    
    }
    bool Saveable::LoadFromPath(FileSystem* fileSystem, const Sequence<char>& path) {
        return fileSystem && LoadFromPath(*fileSystem, path);    
    }
    bool Saveable::LoadFromSequence(const Sequence<uint8_t>& sequence) {
        SequenceReadable readable = sequence;
        return Load(readable);
    }
}