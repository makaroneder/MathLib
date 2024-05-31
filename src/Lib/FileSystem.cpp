#include "FileSystem.hpp"

size_t FileSystem::GetFileSize(size_t file) {
    const size_t pos = Tell(file);
    if (!Seek(file, 0, SeekMode::End)) return SIZE_MAX;
    const size_t size = Tell(file);
    return Seek(file, pos, SeekMode::Set) ? size : SIZE_MAX;
}
String FileSystem::ReadUntil(size_t file, char end) {
    String ret;
    const size_t size = GetFileSize(file);
    while (Tell(file) < size) {
        const char chr = Getc(file);
        if (chr == end) break;
        ret += chr;
    }
    return ret;
}
bool FileSystem::Puts(size_t file, String str) {
    return Write(file, str.ToString(), str.GetSize());
}
char FileSystem::Getc(size_t file) {
    char chr;
    return Read(file, &chr, sizeof(char)) ? chr : '\0';
}