#include "MemoryFileSystem.hpp"

namespace MathLib {
    MemoryFileSystem::MemoryFileSystem(void) : data(""_M, MemoryFileSystemEntry::Type::Directory), files() {}
    MemoryFileSystem::MemoryFileSystem(const MemoryFileSystemEntry& data) : data(data), files() {}
    bool MemoryFileSystem::IsValid(void) const {
        return data.type == MemoryFileSystemEntry::Type::Directory;
    }
    size_t MemoryFileSystem::OpenInternal(const Sequence<char>& path, OpenMode mode) {
        MemoryFileSystemEntry* const ret = mode == OpenMode::Write ? data.Create(path, MemoryFileSystemEntry::Type::File, true) : data.Find(path);
        if (!ret || ret->type != MemoryFileSystemEntry::Type::File) return SIZE_MAX;
        const size_t size = files.GetSize();
        for (size_t i = 0; i < size; i++) {
            if (files.AtUnsafe(i)) continue;
            files.AtUnsafe(i) = ret;
            return i;
        }
        return files.Add(ret) ? size : SIZE_MAX;
    }
    bool MemoryFileSystem::Close(size_t file) {
        if (file >= files.GetSize() || files.AtUnsafe(file)) return false;
        files.AtUnsafe(file) = nullptr;
        return true;
    }
    size_t MemoryFileSystem::Read(size_t file, void* buffer, size_t size, size_t position) {
        return file < files.GetSize() && files.AtUnsafe(file) ? files.AtUnsafe(file)->ReadPositionedSizedBuffer(buffer, size, position) : 0;
    }
    size_t MemoryFileSystem::Write(size_t file, const void* buffer, size_t size, size_t position) {
        return file < files.GetSize() && files.AtUnsafe(file) ? files.AtUnsafe(file)->WritePositionedSizedBuffer(buffer, size, position) : 0;
    }
    size_t MemoryFileSystem::GetSize(size_t file) {
        return file < files.GetSize() && files.AtUnsafe(file) ? files.AtUnsafe(file)->GetSize() : 0;
    }
    Array<FileInfo> MemoryFileSystem::ReadDirectory(const Sequence<char>& path) {
        return data.ReadDirectory(path);
    }
    bool MemoryFileSystem::CreateDirectory(const Sequence<char>& path, bool overwrite) {
        return data.Create(path, MemoryFileSystemEntry::Type::Directory, overwrite);
    }
}