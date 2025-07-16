#ifndef MathLib_SeekMode_H
#define MathLib_SeekMode_H
#include <stdint.h>

namespace MathLib {
    /// @brief Mode to use when setting position
    enum class SeekMode : uint8_t {
        Set,
        Current,
        End,
    };
}

#endif