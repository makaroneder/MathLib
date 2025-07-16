#ifndef MathLib_FileSystem_Path_H
#define MathLib_FileSystem_Path_H
#include "../Typedefs.hpp"

namespace MathLib {
    [[nodiscard]] String RemoveLastPathElement(const Sequence<char>& path);
    [[nodiscard]] String AppendPath(const Sequence<char>& path1, const Sequence<char>& path2);
}

#endif