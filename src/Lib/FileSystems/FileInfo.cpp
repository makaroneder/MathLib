#include "FileInfo.hpp"

namespace MathLib {
    FileInfo::FileInfo(Type type, const String& path) : type(type), path(path) {}
    String FileInfo::ToString(const String& padding) const {
        return padding + path + " [" + typeStr[(size_t)type] + ']';
    }
}