#include "FileInfo.hpp"

FileInfo::FileInfo(const Type& type, const String& path) : type(type), path(path) {}
String FileInfo::ToString(const String& padding) const {
    return padding + path + " [" + typeStr[(size_t)type] + ']';
}