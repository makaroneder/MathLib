#include "FileSystem.hpp"
#include "../String.hpp"

namespace MathLib {
    File FileSystem::Open(const Sequence<char>& path, OpenMode mode) {
        return File(*this, OpenInternal(path, mode));
    }
    String FileSystem::ListFiles(const Sequence<char>& path_, size_t maxDepth, const Sequence<char>& padding) {
        const String path = (path_.GetSize() && path_.At(path_.GetSize() - 1) == '/') ? SubString(path_, 0, path_.GetSize() - 1) : CollectionToString(path_);
        const Array<FileInfo> data = ReadDirectory(path);
        String ret;
        for (const FileInfo& fileInfo : data) {
            ret += CollectionToString(padding) + fileInfo.ToString() + '\n';
            if (fileInfo.type == FileInfo::Type::Directory && maxDepth) ret += ListFiles(path + '/' + fileInfo.path, maxDepth - 1, CollectionToString(padding) + '\t');
        }
        return ret;
    }
}