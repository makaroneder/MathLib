#include "File.hpp"

File FileSystem::Open(const String& path, OpenMode mode) {
    return File(*this, OpenInternal(path, mode));
}
String FileSystem::ListFiles(const String& path_, const String& padding) {
    const String path = (path_.GetSize() && path_.At(path_.GetSize() - 1) == '/') ? SubString(path_, 0, path_.GetSize() - 1) : path_;
    const Array<FileInfo> data = ReadDirectory(path);
    String ret;
    for (const FileInfo& fileInfo : data) {
        ret += padding + fileInfo.ToString() + '\n';
        if (fileInfo.type == FileInfo::Type::Directory) ret += ListFiles(path + '/' + fileInfo.path, padding + '\t');
    }
    return ret;
}