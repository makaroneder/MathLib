#include "File.hpp"

File FileSystem::Open(const String& path, const OpenMode& mode) {
    return File(*this, OpenInternal(path, mode));
}