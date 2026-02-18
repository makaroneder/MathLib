#include "FileInfo.hpp"

namespace MathLib {
    FileInfo::FileInfo(Type type, const Sequence<char>& path) : path(CollectionToString(path)), type(type) {}
    String FileInfo::ToString(const Sequence<char>& padding) const {
        return CollectionToString(padding) + path + " [" + typeStr[(size_t)type] + ']';
    }
    bool FileInfo::operator==(const FileInfo& other) const {
        return type == other.type && path == other.path;
    }
    bool FileInfo::operator!=(const FileInfo& other) const {
        return !(*this == other);
    }
}