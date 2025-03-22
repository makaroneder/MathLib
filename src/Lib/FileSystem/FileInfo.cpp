#include "FileInfo.hpp"

namespace MathLib {
    FileInfo::FileInfo(Type type, const Sequence<char>& path) : type(type), path(CollectionToString(path)) {
        EmptyBenchmark
    }
    String FileInfo::ToString(const Sequence<char>& padding) const {
        StartAndReturnFromBenchmark(CollectionToString(padding) + path + " [" + typeStr[(size_t)type] + ']');
    }
}