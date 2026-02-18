#include "CachedFileSystem.hpp"

namespace MathLib {
    CachedFileSystem::CachedFileSystem(FileSystem& cache, FileSystem& target) : cache(cache), target(target) {}
    size_t CachedFileSystem::OpenInternal(const Sequence<char>& path, OpenMode mode) {
        if (mode == OpenMode::Write) return cache.OpenInternal(path, mode);
        const size_t tmp = cache.OpenInternal(path, mode);
        if (tmp != SIZE_MAX) return tmp;
        do {
            File src = target.Open(path, OpenMode::Read);
            File dst = cache.Open(path, OpenMode::Write);
            if (!src.CopyTo(dst)) return SIZE_MAX;
        } while (false);
        return cache.OpenInternal(path, mode);
    }
    bool CachedFileSystem::Close(size_t file) {
        return cache.Close(file);
    }
    size_t CachedFileSystem::Read(size_t file, void* buffer, size_t size, size_t position) {
        return cache.Read(file, buffer, size, position);
    }
    size_t CachedFileSystem::Write(size_t file, const void* buffer, size_t size, size_t position) {
        return cache.Write(file, buffer, size, position);
    }
    size_t CachedFileSystem::GetSize(size_t file) {
        return cache.GetSize(file);
    }
    Array<FileInfo> CachedFileSystem::ReadDirectory(const Sequence<char>& path) {
        const Array<FileInfo> tmp = target.ReadDirectory(path);
        Array<FileInfo> ret = cache.ReadDirectory(path);
        for (const FileInfo& info : tmp)
            if (!ret.AddUnique(info)) return Array<FileInfo>();
        return ret;
    }
    bool CachedFileSystem::CreateDirectory(const Sequence<char>& path, bool overwrite) {
        return !overwrite && cache.CreateDirectory(path, false);
    }
    bool CachedFileSystem::Commit(void) {
        const Array<FileInfo> files = cache.GetContentsOfDirectory(""_M);
        for (const FileInfo& file : files) switch (file.type) {
            case FileInfo::Type::File: {
                File src = cache.Open(file.path, OpenMode::Read);
                File dst = target.Open(file.path, OpenMode::Write);
                if (!src.CopyTo(dst)) return false;
                break;
            }
            case FileInfo::Type::Directory: {
                if (!target.CreateDirectory(file.path, false)) return false;
                break;
            }
            default: return false;
        }
        return true;
    }
    bool CachedFileSystem::Revert(void) {
        return cache.CreateDirectory(""_M, true);
    }
}