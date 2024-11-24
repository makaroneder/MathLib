#ifndef SeekMode_H
#define SeekMode_H

namespace MathLib {
    /// @brief Mode to use when setting position
    enum class SeekMode {
        Set,
        Current,
        End,
    };
}

#endif