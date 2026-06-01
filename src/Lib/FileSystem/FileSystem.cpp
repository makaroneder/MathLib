#include "FileSystem.hpp"
#include "../String.hpp"
#include "Path.hpp"

namespace MathLib {
    File FileSystem::Open(const Sequence<char>& path, OpenMode mode) {
        return File(*this, OpenInternal(path, mode));
    }
    String FileSystem::ListFiles(const Sequence<char>& path_, size_t maxDepth, const Sequence<char>& padding) {
        const String path = (path_.GetSize() && path_.At(path_.GetSize() - 1) == '/') ? SubString(path_, 0, path_.GetSize() - 1) : CollectionToString(path_);
        const Array<FileInfo> data = ReadDirectory(path);
        const String padd = CollectionToString(padding);
        const String padd2 = padd + '\t';
        String ret;
        for (const FileInfo& fileInfo : data) {
            ret += padd + fileInfo.ToString() + '\n';
            if (fileInfo.type == FileInfo::Type::Directory && maxDepth) ret += ListFiles(AppendPath(path, fileInfo.path), maxDepth - 1, padd2 + '\t');
        }
        return ret;
    }
    bool FileSystem::ListFiles(Writable& logger, const Sequence<char>& path_, size_t maxDepth, const Sequence<char>& padding) {
        const String path = (path_.GetSize() && path_.At(path_.GetSize() - 1) == '/') ? SubString(path_, 0, path_.GetSize() - 1) : CollectionToString(path_);
        const Array<FileInfo> data = ReadDirectory(path);
        const String padd = CollectionToString(padding);
        const String padd2 = padd + '\t';
        for (const FileInfo& fileInfo : data) {
            if (!(logger.Puts(padd) && logger.Puts(fileInfo.ToString()) && logger.Puts('\n'))) return false;
            if (fileInfo.type == FileInfo::Type::Directory && maxDepth && !ListFiles(logger, AppendPath(path, fileInfo.path), maxDepth - 1, padd2)) return false;
        }
        return true;
    }
    bool FileSystem::ListFiles(Writable* logger, const Sequence<char>& path, size_t maxDepth, const Sequence<char>& padding) {
        return logger && ListFiles(*logger, path, maxDepth, padding);
    }
    Array<FileInfo> FileSystem::GetContentsOfDirectory(const Sequence<char>& path_) {
        const String path = (path_.GetSize() && path_.At(path_.GetSize() - 1) == '/') ? SubString(path_, 0, path_.GetSize() - 1) : CollectionToString(path_);
        const Array<FileInfo> infos = ReadDirectory(path);
        Array<FileInfo> ret;
        for (const FileInfo& info : infos) {
            if (!ret.Add(info)) return Array<FileInfo>();
            if (info.type == FileInfo::Type::Directory) {
                const Array<FileInfo> tmp = GetContentsOfDirectory(AppendPath(path, info.path));
                for (const FileInfo& str : tmp)
                    if (!ret.Add(FileInfo(str.type, AppendPath(info.path, str.path)))) return Array<FileInfo>();
            }
        }
        return ret;
    }
}