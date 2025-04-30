#ifndef MathLib_FileSystem_OpenMode_H
#define MathLib_FileSystem_OpenMode_H
#include <stdint.h>

namespace MathLib {
    /// @brief Mode to open file in
    enum class OpenMode : uint8_t {
        Read,
        Write,
        ReadWrite,
    };
}

#endif