#include "MathFS.hpp"
#include "../../String.hpp"
#include "MathFSBootSector.hpp"
#include "MathFSFileHeader.hpp"

MathFS::MathFS(ByteDevice& disk) : PhysicalFileSystem(disk), root(0) {
    MathFSBootSector bootSector;
    if (!disk.ReadPositioned<MathFSBootSector>(bootSector, 0)) Panic("Failed to read MathFS boot sector");
    if (bootSector.IsValid()) root = bootSector.root;
}
bool MathFS::IsValid(void) const {
    return root;
}
bool MathFS::Create(void) {
    root = 1;
    if (!disk.WritePositioned<MathFSBootSector>(MathFSBootSector(root), 0)) return false;
    return disk.WritePositioned<MathFSFileHeader>(MathFSFileHeader(MathFSFileHeader::Type::Directory, root + 1, 0, ""), root * 512);
}
size_t MathFS::OpenInternal(const String& path, OpenMode mode) {
    MathFSFileHeader prev = MathFSFileHeader();
    disk.ReadPositioned<MathFSFileHeader>(prev, root * 512);
    const Array<String> split = Split(path, "/", false);
    for (const String& name : split) {
        bool found = false;
        if (prev.type != MathFSFileHeader::Type::Directory) return SIZE_MAX;
        for (size_t i = 0; i < prev.size && !found; i += sizeof(MathFSFileHeader)) {
            MathFSFileHeader header = MathFSFileHeader();
            disk.ReadPositioned<MathFSFileHeader>(header, prev.lba * 512 + i);
            if (String(header.name) == name) {
                prev = header;
                found = true;
            }
            else if (header.type == MathFSFileHeader::Type::LastEntry) break;
        }
        // TODO: Create file
        if (!found) return SIZE_MAX;
    }
    (void)mode;
    // TODO: Store file info
    return SIZE_MAX;
}
bool MathFS::Close(size_t file) {
    // TODO:
    (void)file;
    return false;
}
size_t MathFS::Read(size_t file, void* buffer, size_t size, size_t position) {
    // TODO:
    (void)file;
    (void)buffer;
    (void)size;
    (void)position;
    return 0;
}
size_t MathFS::Write(size_t file, const void* buffer, size_t size, size_t position) {
    // TODO:
    (void)file;
    (void)buffer;
    (void)size;
    (void)position;
    return 0;
}
size_t MathFS::GetSize(size_t file) {
    // TODO:
    (void)file;
    return 0;
}
Array<FileInfo> MathFS::ReadDirectory(const String& path) {
    // TODO:
    (void)path;
    return Array<FileInfo>();
}