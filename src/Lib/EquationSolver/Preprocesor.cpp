#include "Preprocesor.hpp"
#include "FileSystems/File.hpp"

String Preproces(FileSystem& fileSystem, const String& path) {
    File file = fileSystem.Open(path, OpenMode::Read);
    String ret = "";
    const size_t size = file.GetSize();
    while (file.Tell() < size) ret += file.ReadUntil('\n') + '\n';
    return ret;
}