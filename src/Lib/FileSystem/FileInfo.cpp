#include "FileInfo.hpp"

namespace MathLib {
    FileInfo::FileInfo(Type type, const Sequence<char>& path) : path(CollectionToString(path)), type(type) {
        EmptyBenchmark
    }
    String FileInfo::ToString(const Sequence<char>& padding) const {
        StartAndReturnFromBenchmark(CollectionToString(padding) + path + " [" + typeStr[(size_t)type] + ']');
    }
    bool FileInfo::operator==(const FileInfo& other) const {
        StartAndReturnFromBenchmark(type == other.type && path == other.path);
    }
    bool FileInfo::operator!=(const FileInfo& other) const {
        StartAndReturnFromBenchmark(!(*this == other));
    }
}