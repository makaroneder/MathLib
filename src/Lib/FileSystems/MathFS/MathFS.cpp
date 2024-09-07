#include "MathFS.hpp"
#include "MathFSBootSector.hpp"

MathFS::MathFS(ByteDevice& disk) : PhysicalFileSystem(disk), root(0) {
    MathFSBootSector bootSector;
    if (!disk.ReadPositioned<MathFSBootSector>(bootSector, 0)) Panic("Failed to read MathFS boot sector");
    if (bootSector.IsValid()) root = bootSector.root;
}
bool MathFS::IsValid(void) const {
    // TODO:
    return false;
}
bool MathFS::Create(void) {
    // TODO:
    return false;
}
size_t MathFS::OpenInternal(const String& path, const OpenMode& mode) {
    // TODO:
    (void)path;
    (void)mode;
    return SIZE_MAX;
}
bool MathFS::Close(const size_t& file) {
    // TODO:
    (void)file;
    return false;
}
size_t MathFS::Read(const size_t& file, void* buffer, const size_t& size, const size_t& position) {
    // TODO:
    (void)file;
    (void)buffer;
    (void)size;
    (void)position;
    return 0;
}
size_t MathFS::Write(const size_t& file, const void* buffer, const size_t& size, const size_t& position) {
    // TODO:
    (void)file;
    (void)buffer;
    (void)size;
    (void)position;
    return 0;
}
size_t MathFS::GetSize(const size_t& file) {
    // TODO:
    (void)file;
    return 0;
}
Array<FileInfo> MathFS::ReadDirectory(const String& path) {
    // TODO:
    (void)path;
    return Array<FileInfo>();
}