#include "FileInfo.hpp"

namespace MathLib {
    FileInfo::FileInfo(Type type, const Sequence<char>& path) : path(CollectionToString(path)), type(type) {}
    String FileInfo::ToString(const Sequence<char>& padding) const {
        return CollectionToString(padding) + path + " [" + typeStr[(size_t)type] + ']';
    }
    bool FileInfo::Equals(const FileInfo& other) const {
        return path == other.path;
    }
}