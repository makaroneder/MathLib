#include "FileSystemInfo.hpp"

FileSystemInfo::FileSystemInfo(void) : name(), fs(nullptr) {}
FileSystemInfo::FileSystemInfo(MathLib::FileSystem* fs, const MathLib::String& name) : name(name), fs(fs) {}
MathLib::FileSystem* FileSystemInfo::ReleaseFileSystem(void) {
    MathLib::FileSystem* const ret = fs;
    fs = nullptr;
    return ret;
}
bool FileSystemInfo::IsValid(void) const {
    return fs->IsValid();
}
size_t FileSystemInfo::OpenInternal(const MathLib::Sequence<char>& path, MathLib::OpenMode mode) {
    return fs->OpenInternal(path, mode);
}
bool FileSystemInfo::Close(size_t file) {
    return fs->Close(file);
}
size_t FileSystemInfo::Read(size_t file, void* buffer, size_t size, size_t position) {
    return fs->Read(file, buffer, size, position);
}
size_t FileSystemInfo::Write(size_t file, const void* buffer, size_t size, size_t position) {
    return fs->Write(file, buffer, size, position);
}
size_t FileSystemInfo::GetSize(size_t file) {
    return fs->GetSize(file);
}
MathLib::Array<MathLib::FileInfo> FileSystemInfo::ReadDirectory(const MathLib::Sequence<char>& path) {
    return fs->ReadDirectory(path);
}
bool FileSystemInfo::CreateDirectory(const MathLib::Sequence<char>& path, bool overwrite) {
    return fs->CreateDirectory(path, overwrite);
}
bool FileSystemInfo::Equals(const FileSystemInfo& other) const {
    return name == other.name;
}