#include "Preprocesor.hpp"

// TODO: Add includes and other preprocesor instructions
String Preproces(FileSystem& fileSystem, String path) {
    const size_t file = fileSystem.Open(path, OpenMode::Read);
    String ret = "";
    if (file != SIZE_MAX) {
        const size_t size = fileSystem.GetFileSize(file);
        while (fileSystem.Tell(file) < size) ret += fileSystem.ReadUntil(file, '\n') + '\n';
    }
    return ret;
}