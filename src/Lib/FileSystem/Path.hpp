#ifndef MathLib_FileSystem_Path_H
#define MathLib_FileSystem_Path_H
#include "../Typedefs.hpp"
#include "../SingleTypePair.hpp"

namespace MathLib {
    [[nodiscard]] String RemoveLastPathElement(const Sequence<char>& path);
    [[nodiscard]] String AppendPath(const Sequence<char>& path1, const Sequence<char>& path2);
    [[nodiscard]] SingleTypePair<String> PopFirstPathElement(const Sequence<char>& path);
    [[nodiscard]] SingleTypePair<String> RemoveBasePathAndPopFirstPathElement(const Sequence<char>& basePath, const Sequence<char>& path);
}

#endif