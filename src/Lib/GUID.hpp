#ifndef GUID_H
#define GUID_H
#include "Interfaces/Printable.hpp"
#include <stdint.h>

struct RawGUID {
    uint32_t data1;
    uint16_t data2[3];
    uint64_t data3 : 6 * 8;
} __attribute__((packed));
struct GUID : RawGUID, Printable {
    GUID(RawGUID guid = RawGUID());
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    virtual String ToString(const String& padding = "") const override;
};

#endif