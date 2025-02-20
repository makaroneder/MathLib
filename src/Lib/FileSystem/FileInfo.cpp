#include "FileInfo.hpp"

namespace MathLib {
    FileInfo::FileInfo(Type type, const String& path) : type(type), path(path) {
        EmptyBenchmark
    }
    String FileInfo::ToString(const String& padding) const {
        StartAndReturnFromBenchmark(padding + path + " [" + typeStr[(size_t)type] + ']');
    }
}