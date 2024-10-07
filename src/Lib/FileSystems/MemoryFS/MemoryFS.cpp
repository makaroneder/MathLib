#include "MemoryFS.hpp"

MemoryFS::MemoryFS(void) : resources(Array<MemoryFSResource>()), files(Array<MemoryFSFile>()) {}
size_t MemoryFS::OpenInternal(const String& path, OpenMode mode) {
    size_t resource = SIZE_MAX;
    for (size_t i = 0; i < resources.GetSize() && resource == SIZE_MAX; i++)
        if (resources.At(i).name == path) resource = i;
    if (mode == OpenMode::Write) {
        if (resource == SIZE_MAX) {
            if (!resources.Add(MemoryFSResource(path))) return SIZE_MAX;
            resource = resources.GetSize() - 1;
        }
        resources.At(resource).buffer = Array<uint8_t>();
    }
    if (resource == SIZE_MAX) return SIZE_MAX;
    const MemoryFSFile ret = MemoryFSFile(resource, mode != OpenMode::Read);
    for (size_t i = 0; i < files.GetSize(); i++) {
        if (files.At(i).free) {
            files.At(i) = ret;
            return i;
        }
    }
    return files.Add(ret) ? files.GetSize() - 1 : SIZE_MAX;
}
bool MemoryFS::Close(size_t file) {
    if (files.GetSize() <= file || resources.GetSize() <= files.At(file).resource) return false;
    files.At(file).free = true;
    return true;
}
size_t MemoryFS::Read(size_t file, void* buffer, size_t size, size_t position) {
    if (files.GetSize() <= file || resources.GetSize() <= files.At(file).resource) return 0;
    const size_t s = resources.At(files.At(file).resource).buffer.GetSize() < size + position ? resources.At(files.At(file).resource).buffer.GetSize() - position : size;
    uint8_t* buff8 = (uint8_t*)buffer;
    for (size_t i = 0; i < s; i++) buff8[i] = resources.At(files.At(file).resource).buffer.At(i + position);
    return s;
}
size_t MemoryFS::Write(size_t file, const void* buffer, size_t size, size_t position) {
    if (files.GetSize() <= file || resources.GetSize() <= files.At(file).resource || !files.At(file).write) return 0;
    const size_t s = resources.At(files.At(file).resource).buffer.GetSize() < size + position ? resources.At(files.At(file).resource).buffer.GetSize() - position : size;
    const uint8_t* buff8 = (const uint8_t*)buffer;
    for (size_t i = 0; i < s; i++) resources.At(files.At(file).resource).buffer.At(i + position) = buff8[i];
    return s;
}
size_t MemoryFS::GetSize(size_t file) {
    if (files.GetSize() <= file || resources.GetSize() <= files.At(file).resource) return 0;
    return resources.At(files.At(file).resource).buffer.GetSize();
}
Array<FileInfo> MemoryFS::ReadDirectory(const String& path) {
    if (path.IsEmpty() || path == "/") {
        Array<FileInfo> ret = Array<FileInfo>(resources.GetSize());
        for (size_t i = 0; i < ret.GetSize(); i++) ret.At(i) = FileInfo(FileInfo::Type::File, resources.At(i).name);
        return ret;
    }
    else return Array<FileInfo>();
}