#include "Preprocesor.hpp"
#include "FileSystem/File.hpp"

namespace MathLib {
    String Preproces(FileSystem& fileSystem, const Sequence<char>& path) {
        File file = fileSystem.Open(path, OpenMode::Read);
        String ret = "";
        const size_t size = file.GetSize();
        while (file.Tell() < size) ret += file.ReadUntil('\n') + '\n';
        return ret;
    }
}