#include "Saveable.hpp"
#include "FileSystem/File.hpp"

namespace MathLib {
    bool Saveable::SaveFromPath(FileSystem& fileSystem, const Sequence<char>& path) const {
        StartBenchmark
        File file = fileSystem.Open(path, OpenMode::Write);
        ReturnFromBenchmark(Save(file));
    }
    bool Saveable::LoadFromPath(FileSystem& fileSystem, const Sequence<char>& path) {
        StartBenchmark
        File file = fileSystem.Open(path, OpenMode::Read);
        ReturnFromBenchmark(Load(file));
    }
}