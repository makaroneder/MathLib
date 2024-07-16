#ifndef Emulator_Register_H
#define Emulator_Register_H
#include "../Printable.hpp"

struct Register : Printable {
    Register(const uint64_t& val = 0);
    uint32_t Get32(const bool& upper) const;
    uint16_t Get16(const bool& upper) const;
    uint8_t Get8(const bool& upper) const;
    void Set32(const uint32_t& val, const bool& upper);
    void Set16(const uint16_t& val, const bool& upper);
    void Set8(const uint8_t& val, const bool& upper);
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    virtual String ToString(const String& padding = "") const override;

    uint64_t value;
};

#endif