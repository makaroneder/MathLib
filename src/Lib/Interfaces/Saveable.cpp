#include "Saveable.hpp"
#include "FileSystem/File.hpp"

namespace MathLib {
    bool Saveable::SaveFromPath(FileSystem& fileSystem, const String& path) const {
        StartBenchmark
        File file = fileSystem.Open(path, OpenMode::Write);
        ReturnFromBenchmark(Save(file));
    }
    bool Saveable::LoadFromPath(FileSystem& fileSystem, const String& path) {
        StartBenchmark
        File file = fileSystem.Open(path, OpenMode::Read);
        ReturnFromBenchmark(Load(file));
    }
}