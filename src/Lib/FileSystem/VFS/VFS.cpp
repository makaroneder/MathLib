#include "VFS.hpp"
#include "../Path.hpp"
#include "../../String.hpp"

namespace MathLib {
    VFS::VFS(void) : entries(), files() {}
    bool VFS::AddFileSystem(const VFSEntry& entry) {
        return entry.fs && entries.Add(entry);
    }
    Pair<String, size_t> VFS::GetMountpoint(const Sequence<char>& path) const {
        const size_t entryCount = entries.GetSize();
        String remainder;
        size_t fs = SIZE_MAX;
        for (size_t i = 0; i < entryCount; i++) {
            const VFSEntry entry = entries.AtUnsafe(i);
            if (entry.removed || !path.StartsWith(entry.mountpoint)) continue;
            const String tmp = RemoveBasePath(entry.mountpoint, path);
            if (fs != SIZE_MAX && tmp.GetSize() >= remainder.GetSize()) continue;
            remainder = tmp;
            fs = i;
        }
        return Pair<String, size_t>(remainder, fs);
    }
    bool VFS::IsValid(void) const {
        for (const VFSEntry& entry : entries)
            if (!(entry.fs && entry.fs->IsValid())) return false;
        return true;
    }
    size_t VFS::OpenInternal(const Sequence<char>& path, OpenMode mode) {
        const Pair<String, size_t> mount = GetMountpoint(path);
        const VFSFile ret = VFSFile(mount.second, entries.AtUnsafe(mount.second).fs->OpenInternal(mount.first, mode));
        if (ret.fs == SIZE_MAX) return SIZE_MAX;
        if (ret.index == SIZE_MAX) return SIZE_MAX;
        const size_t size = files.GetSize();
        for (size_t i = 0; i < size; i++) {
            if (!files.AtUnsafe(i).free) continue;
            files.AtUnsafe(i) = ret;
            return i;
        }
        return files.Add(ret) ? size : SIZE_MAX;
    }
    bool VFS::Close(size_t file) {
        if (!IsValid(file) || !entries.AtUnsafe(files.AtUnsafe(file).fs).fs->Close(files.AtUnsafe(file).index)) return false;
        files.AtUnsafe(file).free = true;
        return true;
    }
    size_t VFS::Read(size_t file, void* buffer, size_t size, size_t position) {
        return IsValid(file) ? entries.AtUnsafe(files.AtUnsafe(file).fs).fs->Read(files.AtUnsafe(file).index, buffer, size, position) : 0;
    }
    size_t VFS::Write(size_t file, const void* buffer, size_t size, size_t position) {
        return IsValid(file) ? entries.AtUnsafe(files.AtUnsafe(file).fs).fs->Write(files.AtUnsafe(file).index, buffer, size, position) : 0;
    }
    size_t VFS::GetSize(size_t file) {
        return IsValid(file) ? entries.AtUnsafe(files.AtUnsafe(file).fs).fs->GetSize(files.AtUnsafe(file).index) : 0;
    }
    Array<FileInfo> VFS::ReadDirectory(const Sequence<char>& path) {
        const Pair<String, size_t> mount = GetMountpoint(path);
        Array<FileInfo> ret = mount.second != SIZE_MAX ? entries.AtUnsafe(mount.second).fs->ReadDirectory(mount.first) : Array<FileInfo>();
        for (const VFSEntry& entry : entries) {
            if (entry.removed || !entry.mountpoint.StartsWith(path)) continue;
            const String tmp = RemoveBasePath(path, entry.mountpoint);
            if (tmp.IsEmpty()) continue;
            if (!ret.AddUnique(FileInfo(FileInfo::Type::Directory, PopFirstPathElement(tmp).first))) return Array<FileInfo>();
        }
        return ret;
    }
    bool VFS::CreateDirectory(const Sequence<char>& path, bool overwrite) {
        for (VFSEntry& entry : entries)
            if (entry.mountpoint == path) entry.removed = false;
        const Pair<String, size_t> mount = GetMountpoint(path);
        bool created = mount.second != SIZE_MAX;
        if (created && !entries.AtUnsafe(mount.second).fs->CreateDirectory(mount.first, overwrite)) return false;
        for (VFSEntry& entry : entries) {
            if (entry.removed) continue;
            const String tmp = RemoveBasePath(path, entry.mountpoint);
            if (tmp.IsEmpty()) continue;
            if (!entry.fs->CreateDirectory(""_M, true)) return false;
            entry.removed = true;
            created = true;
        }
        return created;
    }
    bool VFS::IsValid(size_t file) const {
        return file < files.GetSize() && files.AtUnsafe(file).fs < entries.GetSize();
    }
}