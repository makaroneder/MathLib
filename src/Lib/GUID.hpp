#ifndef MathLib_GUID_H
#define MathLib_GUID_H
#include "Interfaces/Printable.hpp"

namespace MathLib {
    struct RawGUID {
        uint32_t data1;
        uint16_t data2[3];
        uint64_t data3 : 48;
    } __attribute__((packed));
    struct GUID : RawGUID, Printable {
        GUID(RawGUID guid = RawGUID());
        /// @brief Converts struct to string
        /// @param padding String to pad with
        /// @return String representation
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override;
    };
}

#endif