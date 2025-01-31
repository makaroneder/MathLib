#include "File.hpp"
#include "../String.hpp"

namespace MathLib {
    File FileSystem::Open(const String& path, OpenMode mode) {
        StartBenchmark
        ReturnFromBenchmark(File(*this, OpenInternal(path, mode)));
    }
    String FileSystem::ListFiles(const String& path_, size_t maxDepth, const String& padding) {
        StartBenchmark
        const String path = (path_.GetSize() && path_.At(path_.GetSize() - 1) == '/') ? SubString(path_, 0, path_.GetSize() - 1) : path_;
        const Array<FileInfo> data = ReadDirectory(path);
        String ret;
        for (const FileInfo& fileInfo : data) {
            ret += padding + fileInfo.ToString() + '\n';
            if (fileInfo.type == FileInfo::Type::Directory && maxDepth) ret += ListFiles(path + '/' + fileInfo.path, maxDepth - 1, padding + '\t');
        }
        ReturnFromBenchmark(ret);
    }
}