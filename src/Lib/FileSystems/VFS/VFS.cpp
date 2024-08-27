#include "VFS.hpp"
#include "../../String.hpp"

VFS::VFS(void) : entries(0), files(0) {}
size_t VFS::GetFileSystems(void) const {
    return entries.GetSize();
}
bool VFS::AddFileSystem(const VFSEntry& entry) {
    return entries.Add(entry);
}
size_t VFS::OpenInternal(const String& path, const OpenMode& mode) {
    size_t off = 0;
    if (path.At(0) == '/') off++;
    String fs;
    for (; path.At(off) != '/'; off++) fs += path.At(off);
    off++;
    String fsPath = SubString(path, off, path.GetSize() - off);
    size_t fsIndex = SIZE_MAX;
    for (size_t i = 0; i < entries.GetSize() && fsIndex == SIZE_MAX; i++)
        if (entries.At(i).name == fs) fsIndex = i;
    if (fsIndex == SIZE_MAX) return SIZE_MAX;
    const VFSFile ret = VFSFile(fsIndex, entries.At(fsIndex).fs->OpenInternal(fsPath, mode));
    if (ret.index == SIZE_MAX) return SIZE_MAX;
    for (size_t i = 0; i < files.GetSize(); i++) {
        if (files.At(i).free) {
            files.At(i) = ret;
            return i;
        }
    }
    return files.Add(ret) ? files.GetSize() - 1 : SIZE_MAX;
}
bool VFS::Close(const size_t& file) {
    if (files.GetSize() <= file || entries.GetSize() <= files.At(file).fs) return false;
    if (!entries.At(files.At(file).fs).fs->Close(files.At(file).index)) return false;
    files.At(file).free = true;
    return true;
}
size_t VFS::Read(const size_t& file, void* buffer, const size_t& size, const size_t& position) {
    if (files.GetSize() <= file || entries.GetSize() <= files.At(file).fs) return 0;
    return entries.At(files.At(file).fs).fs->Read(files.At(file).index, buffer, size, position);
}
size_t VFS::Write(const size_t& file, const void* buffer, const size_t& size, const size_t& position) {
    if (files.GetSize() <= file || entries.GetSize() <= files.At(file).fs) return 0;
    return entries.At(files.At(file).fs).fs->Write(files.At(file).index, buffer, size, position);
}
size_t VFS::GetSize(const size_t& file) {
    if (files.GetSize() <= file || entries.GetSize() <= files.At(file).fs) return 0;
    return entries.At(files.At(file).fs).fs->GetSize(files.At(file).index);
}
Array<FileInfo> VFS::ReadDirectory(const String& path) {
    size_t off = 0;
    if (!path.IsEmpty() && path.At(0) == '/') off++;
    String fs;
    for (; off < path.GetSize() && path.At(off) != '/'; off++) fs += path.At(off);
    if (fs.IsEmpty()) {
        Array<FileInfo> ret = Array<FileInfo>();
        for (const VFSEntry& entry : entries)
            if (!ret.Add(FileInfo(FileInfo::Type::Directory, entry.name))) return Array<FileInfo>();
        return ret;
    }
    off++;
    String fsPath = off == path.GetSize() + 1 ? "" : SubString(path, off, path.GetSize() - off);
    for (size_t i = 0; i < entries.GetSize(); i++)
        if (entries.At(i).name == fs) return entries.At(i).fs->ReadDirectory(fsPath);
    return Array<FileInfo>();
}