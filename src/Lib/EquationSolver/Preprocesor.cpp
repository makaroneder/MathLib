#include "Preprocesor.hpp"
#include "FileSystem/File.hpp"

namespace MathLib {
    String Preproces(FileSystem& fileSystem, const String& path) {
        StartBenchmark
        File file = fileSystem.Open(path, OpenMode::Read);
        String ret = "";
        const size_t size = file.GetSize();
        while (file.Tell() < size) ret += file.ReadUntil('\n') + '\n';
        ReturnFromBenchmark(ret);
    }
}