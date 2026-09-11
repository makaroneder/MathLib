#include "SystemCallFileSystem.hpp"
#include "SystemCall.hpp"
#include <OpenSystemCallData.hpp>
#include <ReadWriteSystemCallData.hpp>

bool SystemCallFileSystem::IsValid(void) const {
    return true;
}
size_t SystemCallFileSystem::OpenInternal(const MathLib::Sequence<char>& path, MathLib::OpenMode mode) {
    const size_t size = path.GetSize();
    char buff[size + 1];
    for (size_t i = 0; i < size; i++) buff[i] = path.AtUnsafe(i);
    buff[size] = '\0';
    return PerformSystemCall<OpenSystemCallData>(SystemCall::OpenFile, OpenSystemCallData(buff, mode == MathLib::OpenMode::Write));
}
bool SystemCallFileSystem::Close(size_t file) {
    return PerformSystemCall<uint64_t>(SystemCall::CloseFile, file);
}
size_t SystemCallFileSystem::Read(size_t file, void* buffer, size_t size, size_t position) {
    return PerformSystemCall<ReadWriteSystemCallData>(SystemCall::ReadFile, ReadWriteSystemCallData(file, (uint64_t)buffer, size, position));
}
size_t SystemCallFileSystem::Write(size_t file, const void* buffer, size_t size, size_t position) {
    return PerformSystemCall<ReadWriteSystemCallData>(SystemCall::WriteFile, ReadWriteSystemCallData(file, (uint64_t)buffer, size, position));
}
size_t SystemCallFileSystem::GetSize(size_t file) {
    return PerformSystemCall<uint64_t>(SystemCall::GetSizeOfFile, file);
}
MathLib::Array<MathLib::FileInfo> SystemCallFileSystem::ReadDirectory(const MathLib::Sequence<char>& path) {
    // TODO:
    (void)path;
    return MathLib::Array<MathLib::FileInfo>();
}
bool SystemCallFileSystem::CreateDirectory(const MathLib::Sequence<char>& path, bool overwrite) {
    // TODO:
    (void)path;
    (void)overwrite;
    return false;
}