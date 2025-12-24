#include "Directory.hpp"

namespace MathLib {
    Directory::Directory(FileSystem& base, const Sequence<char>& basePath) : basePath(CollectionToString(basePath)), base(base) {}
    size_t Directory::OpenInternal(const Sequence<char>& path, OpenMode mode) {
        return base.OpenInternal(basePath + '/' + path, mode);
    }
    bool Directory::Close(size_t file) {
        return file != SIZE_MAX && base.Close(file);
    }
    size_t Directory::Read(size_t file, void* buffer, size_t size, size_t position) {
        return file == SIZE_MAX ? 0 : base.Read(file, buffer, size, position);
    }
    size_t Directory::Write(size_t file, const void* buffer, size_t size, size_t position) {
        return file == SIZE_MAX ? 0 : base.Write(file, buffer, size, position);
    }
    size_t Directory::GetSize(size_t file) {
        return file == SIZE_MAX ? 0 : base.GetSize(file);
    }
    Array<FileInfo> Directory::ReadDirectory(const Sequence<char>& path) {
        return base.ReadDirectory(basePath + '/' + path);
    }
    bool Directory::CreateDirectory(const Sequence<char>& path, bool overwrite) {
        return base.CreateDirectory(basePath + '/' + path, overwrite);
    }
}