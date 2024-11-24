#include "VFS.hpp"
#include "../../String.hpp"

namespace MathLib {
    VFS::VFS(void) : entries(Array<VFSEntry>()), files(Array<VFSFile>()) {}
    bool VFS::AddFileSystem(const VFSEntry& entry) {
        return entry.fs && entries.Add(entry);
    }
    size_t VFS::OpenInternal(const String& path, OpenMode mode) {
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
    bool VFS::Close(size_t file) {
        if (files.GetSize() <= file || entries.GetSize() <= files.At(file).fs) return false;
        if (!entries.At(files.At(file).fs).fs->Close(files.At(file).index)) return false;
        files.At(file).free = true;
        return true;
    }
    size_t VFS::Read(size_t file, void* buffer, size_t size, size_t position) {
        if (files.GetSize() <= file || entries.GetSize() <= files.At(file).fs) return 0;
        return entries.At(files.At(file).fs).fs->Read(files.At(file).index, buffer, size, position);
    }
    size_t VFS::Write(size_t file, const void* buffer, size_t size, size_t position) {
        if (files.GetSize() <= file || entries.GetSize() <= files.At(file).fs) return 0;
        return entries.At(files.At(file).fs).fs->Write(files.At(file).index, buffer, size, position);
    }
    size_t VFS::GetSize(size_t file) {
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
        for (size_t i = 0; i < entries.GetSize(); i++)
            if (entries.At(i).name == fs) return entries.At(i).fs->ReadDirectory(++off == path.GetSize() + 1 ? "" : SubString(path, off, path.GetSize() - off));
        return Array<FileInfo>();
    }
}