#include "QuakePakFileEntry.hpp"
#include "QuakePak.hpp"
#include "../Path.hpp"

namespace MathLib {
    QuakePak::QuakePak(ByteDevice& disk) : PhysicalFileSystem(disk) {
        if (!disk.ReadPositioned<QuakePakHeader>(header, 0)) Panic("Failed to read quake pack header");
    }
    bool QuakePak::IsValid(void) const {
        return header.IsValid();
    }
    bool QuakePak::Create(void) {
        const QuakePakHeader tmp = sizeof(QuakePakHeader);
        if (!disk.WritePositioned<QuakePakHeader>(tmp, 0)) return false;
        header = tmp;
        return true;
    }
    size_t QuakePak::OpenInternal(const Sequence<char>& path, OpenMode) {
        if (!disk.Seek(header.fileTablePosition, SeekMode::Set)) return SIZE_MAX;
        const size_t size = files.GetSize();
        for (uint32_t i = 0; i < header.fileTableSize; i += sizeof(QuakePakFileEntry)) {
            QuakePakFileEntry entry;
            if (!disk.Read<QuakePakFileEntry>(entry)) return SIZE_MAX;
            if (entry.GetName() != path) continue;
            const QuakePakFile file = QuakePakFile(entry.position, entry.size);
            for (size_t i = 0; i < size; i++) {
                if (!files.At(i).free) continue;
                files.At(i) = file;
                return i;
            }
            return files.Add(file) ? size : SIZE_MAX;
        }
        return SIZE_MAX;
    }
    bool QuakePak::Close(size_t file) {
        if (file >= files.GetSize() || files.At(file).free) return false;
        files.At(file).free = true;
        return true;
    }
    size_t QuakePak::Read(size_t file, void* buffer, size_t size, size_t position) {
        if (file >= files.GetSize() || files.At(file).free || position >= files.At(file).size) return 0;
        return disk.ReadPositionedSizedBuffer(buffer, (files.At(file).size < position + size ? files.At(file).size - position : size), position + files.At(file).position);
    }
    size_t QuakePak::Write(size_t file, const void* buffer, size_t size, size_t position) {
        if (file >= files.GetSize() || files.At(file).free || position >= files.At(file).size) return 0;
        return disk.WritePositionedSizedBuffer(buffer, (files.At(file).size < position + size ? files.At(file).size - position : size), position + files.At(file).position);
    }
    size_t QuakePak::GetSize(size_t file) {
        return file >= files.GetSize() || files.At(file).free ? 0 : files.At(file).size;
    }
    Array<FileInfo> QuakePak::ReadDirectory(const Sequence<char>& path) {
        if (!disk.Seek(header.fileTablePosition, SeekMode::Set)) return Array<FileInfo>();
        Array<FileInfo> ret;
        for (uint32_t i = 0; i < header.fileTableSize; i += sizeof(QuakePakFileEntry)) {
            QuakePakFileEntry entry;
            if (!disk.Read<QuakePakFileEntry>(entry)) return Array<FileInfo>();
            const SingleTypePair<String> name = RemoveBasePathAndPopFirstPathElement(path, entry.GetName());
            if (name.first.IsEmpty()) continue;
            const FileInfo info = FileInfo(name.second.IsEmpty() ? FileInfo::Type::File : FileInfo::Type::Directory, name.first);
            if (!(ret.Contains(info) || ret.Add(info))) return Array<FileInfo>();
        }
        return ret;
    }
    bool QuakePak::CreateDirectory(const Sequence<char>& path, bool overwrite) {
        // TODO:
        (void)path;
        (void)overwrite;
        return false;
    }
}